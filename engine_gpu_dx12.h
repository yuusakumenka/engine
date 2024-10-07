/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* Hopes and dreams. */

typedef struct Os_Surface Os_Surface;

typedef struct Gpu_Swapchain_Descripion {
    Os_Surface *surface;
    /* vsync */
    size_t width;
    size_t height;
} Gpu_Swapchain_Descripion;

typedef struct Gpu_Device    Gpu_Device;
typedef struct Gpu_Swapchain Gpu_Swapchain;

Gpu_Swapchain *gpu_swapchain_create(
    Gpu_Device               *device,
    Gpu_Swapchain_Descripion *description
) {

}

/* swapchain MUST have been created with a non null surface. */
void gpu_swapchain_present(Gpu_Swapchain *swapchain) {

}