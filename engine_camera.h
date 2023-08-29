typedef float Camera_F32;

typedef struct Camera_V2F32 {
	Camera_F32 x;
	Camera_F32 y;
} Camera_V2F32;

typedef struct Camera_M3x2 {
    Camera_F32 a, b, c;
    Camera_F32 d, e, f;
} Camera_M3x2;

// Camera
typedef struct Camera {

    // The position of the camera in the coordinate space it is framing into.
    Camera_V2F32  position;

    // The aspect-ratio of the camera. For reminder, aspect ratio is width / height.
    Camera_F32 aspect_ratio;

    // The size of the camera in the coordinate space it is framing into.
    // It is relative to an aspect ratio of 1.0, meaning changing the aspect ratio keeps the covered area unchanged.
    Camera_F32 size; // CLARITY: should rename to something like "area" or "sqrt_area"?

    // UPCOMING FEATURE (not yet implemented):
    // How should differencies between camera and screen aspect-ratio be handled:
    // 0.00 -> Pan and Scan: 
    //             The camera rectangle will be fully contained by the screen rectangle.
    //             User won't miss visual information, but you may reveal some graphic space user wasn't suposed to see.
    // 1.00 -> Letterboxing: 
    //             The screen rectangle will be fully covered   by the camera rectangle.
    //             User won't see outside of the expected region but some visual information may be lost oustide the screen.
    // Engine will use this value interpolate linearly between the two effects.
    Camera_F32 pan_and_scan_vs_letterboxing;

} Camera;

V2 camera_letterbox(Camera* camera, F32 viewport_aspect_ratio) {
    F32 size = camera->size;
    if (viewport_aspect_ratio < camera->aspect_ratio) {
        size = (size * camera->aspect_ratio) / viewport_aspect_ratio;
    }
    return v2(
        size * viewport_aspect_ratio * 0.5f,
        size * 0.5f
    );
}

V2 camera_pan_and_scan(Camera* camera, F32 viewport_aspect_ratio);

M3x2 world_to_viewport(Camera* camera, F32 viewport_aspect_ratio) {
    V2 scale         = camera_letterbox(camera, viewport_aspect_ratio);
    V2 inverse_scale = v2(
        1.0f / scale.x,
        1.0f / scale.y
    );
    return m3x2(
        inverse_scale.x, 0              , -camera->position.x * inverse_scale.x,
        0              , inverse_scale.y, -camera->position.y * inverse_scale.y
    );
}
M3x2 viewport_to_world(Camera* camera, F32 viewport_aspect_ratio) {
    V2 scale = camera_letterbox(camera, viewport_aspect_ratio);
    return m3x2(
        scale.x,       0, camera->position.x,
        0      , scale.y, camera->position.y
    );
}