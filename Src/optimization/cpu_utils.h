#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <vector>
#include <thread>
#include <immintrin.h> // Для SIMD инструкций

class CPUOptimizer {
public:
    static void init();
    static void optimizeCache();
    static void vectorizeOperations();
    static void parallelizeRaycasting(int startX, int endX, int startY, int endY);
    static bool shouldOffloadToGPU();
    static void handleGPUOverflow();
    
private:
    static void setupSIMD();
    static void optimizeMemoryAlignment();
    static void distributeWorkload();
    static int calculateOptimalThreadCount();
    static void raycastSegment(int startX, int endX, int startY, int endY);
};

#endif 