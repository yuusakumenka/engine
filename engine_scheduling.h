/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* This is a mingle of task graph and scheduling. Maybe those shouldn't be tied together...
 * We don't have lambda in C so every task has to be it's own procedure.
 * This may desincentivise to write parallel code, so we might introduce fibers and wait_for_counter in the future?
 * Also @SebAaltonen wrote interesting stuff about job stealing, lazy binary splitting,
 * tying thread pools to a single Core Chiplet Die (CCD). We'll be looking into that. */

/* No Job graph builder helper right now.
 * For now you have to write the job graph's memory manually.
 * Here is the scary example: */
#if 0
/* Job Graph  */
{
    engine->worker_init_proc         = worker_init_proc;
    engine->worker_graphic_init_proc = worker_graphic_init_proc;
    Job    *jobs                      = ARENA_INSERT_ARRAY(engine->app_arena, Job, 5);
    Job   **dependers                 = ARENA_INSERT_ARRAY(engine->app_arena, Job*, 4);
    size_t *dependee_counters         = ARENA_INSERT_ARRAY(engine->app_arena, size_t, 4);
    size_t *dependee_counters_initial = ARENA_INSERT_ARRAY(engine->app_arena, size_t, 4);
    Job_Graph_Reset_Job *reset        =
        ARENA_INSERT_STRUCT(engine->app_arena, Job_Graph_Reset_Job);
    Job **jobs_without_dependees = ARENA_INSERT_ARRAY(engine->app_arena, Job*, 1);
    
    jobs[0].proc             = in_backend_swap_buffer_job;
    jobs[0].data             = &in;
    jobs[0].dependee_counter = 0;
    jobs[0].num_dependers    = 1;
    jobs[0].dependers        = &dependers[0];
    jobs[0].dependers[0]     = &jobs[1];
    jobs[0].ring             = &engine->job_ring;

    jobs[1].proc             = engine_main_loop_body;
    jobs[1].data             = 0;
    jobs[1].dependee_counter = &dependee_counters[0];
    (&dependee_counters_initial[0])[0] = 1;
    jobs[1].num_dependers    = 1;
    jobs[1].dependers        = &dependers[1];
    jobs[1].dependers[0]     = &jobs[2];
    jobs[1].ring             = &engine->job_ring_graphics;

    {
        M4 *vps = ARENA_INSERT_ARRAY(engine->app_arena, M4, 2);
        {
            V2 resolution = os_surface_get_resolution(game->main_surface);
            M3x2 world2view = world_to_viewport(
                &engine->camera, resolution.x / resolution.y);
            vps[0] = m4(
                world2view.a, world2view.b, 0, world2view.c,
                world2view.d, world2view.e, 0, world2view.f,
                        0,            0, 0,            0,
                        0,            0, 0,            1
            );
            M3x2 window2viewport = window_to_viewport(game->main_surface);
            vps[1] = m4(
                window2viewport.a, window2viewport.b, 0, window2viewport.c,
                window2viewport.d, window2viewport.e, 0, window2viewport.f,
                                0,                 0, 0,                 0,
                                0,                 0, 0,                 1
            );
        }
        
        Debug_Draw_Renderer_Populate_Command_List_For_Layer_Job *job =
            ARENA_INSERT_STRUCT(
                engine->app_arena,
                Debug_Draw_Renderer_Populate_Command_List_For_Layer_Job
            );
        job->renderer     = &game->debug_draw_renderer;
        job->command_list = engine->gpu_command_allocator;
        job->layer_index  = 1;
        job->vps          = vps;

        /* Backup ptr to job description so we can update it every frame. */
        game->debug_draw_renderer_populate_command_list_for_layer_1_job = job;

        jobs[2].proc             = debug_draw_renderer_populate_command_list_for_layer_job;
        jobs[2].data             = job;
        jobs[2].dependee_counter = &dependee_counters[1];
        (&dependee_counters_initial[1])[0] = 1;
        jobs[2].num_dependers    = 1;
        jobs[2].dependers        = &dependers[2];
        jobs[2].dependers[0]     = &jobs[3];
        jobs[2].ring             = &engine->job_ring_graphics;
    }

    jobs[3].proc             = gpu_present;
    jobs[3].data             = game->main_surface;
    jobs[3].dependee_counter = &dependee_counters[2];
    (&dependee_counters_initial[2])[0] = 1;
    jobs[3].num_dependers    = 1;
    jobs[3].dependers        = &dependers[3];
    jobs[3].dependers[0]     = &jobs[4];
    jobs[3].ring             = &engine->job_ring_graphics;

    jobs[4].proc             = game_frame_begin_job; /* we call job_graph_reset_job in here */
    jobs[4].data             = reset;
    jobs[4].dependee_counter = &dependee_counters[3];
    (&dependee_counters_initial[3])[0] = 1;
    jobs[4].num_dependers    = 0;
    jobs[4].dependers        = 0;
    jobs[4].ring             = &engine->job_ring;

    jobs_without_dependees[0] = &jobs[0];

    reset->num_dependee_counters      = 4;
    reset->dependee_counters          = dependee_counters;
    reset->dependee_counters_initial  = dependee_counters_initial;
    reset->num_jobs_without_dependees = 1;
    reset->jobs_without_dependees     = jobs_without_dependees;

    job_graph_reset_job(reset); /* Kick the job graph. */
}
#endif

typedef struct Job Job;
typedef struct Job_Ring64 Job_Ring64;

typedef struct Job {
    void      (*proc)(void *data);
    void       *data;
    size_t     *dependee_counter;
    size_t      num_dependers;
    Job       **dependers;
    Job_Ring64 *ring;
} Job;

#define JOB_RING64_NUM_JOBS_MAX 64

typedef struct Job_Ring64 {
    Job   *jobs[64];
    size_t head; /* should be put on different cacheline than tail? */
    size_t tail;
} Job_Ring64;

typedef struct Job_Graph_Reset_Job {
    size_t   num_dependee_counters;
    size_t **dependee_counters_initial;
    size_t **dependee_counters;
    size_t   num_jobs_without_dependees;
    Job    **jobs_without_dependees;
} Job_Graph_Reset_Job;

typedef U8 Job_Ring_Error;
typedef enum Job_Ring_Error_ {
    JOB_RING_ERROR_NONE,
    JOB_RING_ERROR_TRYING_TO_INSERT_PAST_MAX_CAPACITY
} Job_Ring_Error_;

size_t job_ring64_job_id_to_index_in_ring(size_t job_id) {
    #if 1 /* Avoid false cache sharing. */
    /* To avoid false cache sharing, we stride each consecutive 
     * job pointer access by one cache line.
     * Since we mostly access consecutive job ids, we'll
     * have a tendency to access consecutive but distinct cache lines. */
    return 
        (job_id & 0x07) << 3 |
        (job_id & 0x38) >> 3;
    #else
    return job_id & (64 - 1); /* Just do a regular modulo. */
    #endif
}

Job_Ring_Error job_ring64_insert_one(Job_Ring64 *ring, Job *job) {
    for(;;) {
        size_t tail = interlocked_compare_exchange(&ring->tail, 0, 0);
        size_t head = interlocked_compare_exchange(&ring->head, 0, 0);
        if(head - tail == JOB_RING64_NUM_JOBS_MAX)
            return JOB_RING_ERROR_TRYING_TO_INSERT_PAST_MAX_CAPACITY;
        if(interlocked_compare_exchange(
            &ring->jobs[job_ring64_job_id_to_index_in_ring(head)],
            job,
            0) == 0
        ) {
            interlocked_increment(&ring->head);
            return JOB_RING_ERROR_NONE;
        }
    }
}

Job *job_ring64_try_remove_one(Job_Ring64 *ring) {
    Job *job;
    for (;;) {
        size_t tail = interlocked_compare_exchange(&ring->tail, 0, 0);
        size_t head = interlocked_compare_exchange(&ring->head, 0, 0);
        if (tail == head) {
            return 0;
        }
        size_t job_index = job_ring64_job_id_to_index_in_ring(tail);
        job = interlocked_compare_exchange(&ring->jobs[job_index], 0, 0);
        if (interlocked_compare_exchange(&ring->jobs[job_index], 0, job) == job) {
            interlocked_increment(&ring->tail);
            return job;
        }
    }
}

void job_end(Job *job) {
    for (size_t depender_index = 0; depender_index < job->num_dependers; ++depender_index) {
        Job   *depender = job->dependers[depender_index];
        size_t depender_dependee_counter_post_decrement =
            interlocked_decrement(depender->dependee_counter);
        if (depender_dependee_counter_post_decrement == 0) {
            job_ring64_insert_one(depender->ring, depender);
        }
    }
}

void job_graph_reset_job(Job_Graph_Reset_Job *job) {
    memcpy(
        job->dependee_counters,
        job->dependee_counters_initial,
        sizeof(size_t) * job->num_dependee_counters
    );
    for(
        size_t job_without_dependees_index = 0;
        job_without_dependees_index != job->num_jobs_without_dependees;
        ++job_without_dependees_index
    ) {
        Job *jobs_without_dependees =
            job->jobs_without_dependees[job_without_dependees_index];
        job_ring64_insert_one(
            jobs_without_dependees->ring,
            jobs_without_dependees
        );
    }
}