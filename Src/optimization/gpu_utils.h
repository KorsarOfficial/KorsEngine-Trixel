#ifndef GPU_UTILS_H
#define GPU_UTILS_H

#include <SDL2/SDL.h>

class GPUOptimizer {
public:
    static void init();
    static bool isAvailable();
    static void optimizeRendering();
    static void useHardwareAcceleration();
    static void handleRenderingOverflow();
    static void balanceWorkload(float cpuLoad, float gpuLoad);
    static float measureGPULoad();
    
private:
    static bool checkGPUCapabilities();
    static void setupRenderingPipeline();
    static void optimizeTextureHandling();
};

#endif 