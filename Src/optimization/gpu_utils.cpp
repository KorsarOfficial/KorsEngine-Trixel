#include "gpu_utils.h"
#include <SDL2/SDL.h>

bool GPUOptimizer::isAvailable() {
    SDL_RendererInfo info;
    return SDL_GetRenderDriverInfo(0, &info) == 0 && 
           (info.flags & SDL_RENDERER_ACCELERATED);
}

void GPUOptimizer::init() {
    if(isAvailable()) {
        useHardwareAcceleration();
        setupRenderingPipeline();
    }
}

void GPUOptimizer::optimizeRendering() {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
}

void GPUOptimizer::useHardwareAcceleration() {
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}

void GPUOptimizer::handleRenderingOverflow() {
    balanceWorkload(0.5f, measureGPULoad());
}

void GPUOptimizer::balanceWorkload(float cpuLoad, float gpuLoad) {
    // Базовая реализация
}

float GPUOptimizer::measureGPULoad() {
    static Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    return deltaTime > 0.016f ? 0.9f : 0.5f;
}

bool GPUOptimizer::checkGPUCapabilities() {
    return isAvailable();
}

void GPUOptimizer::setupRenderingPipeline() {
    optimizeRendering();
}

void GPUOptimizer::optimizeTextureHandling() {
    // Базовая реализация
}