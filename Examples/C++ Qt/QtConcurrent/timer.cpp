#include "timer.h"


Timer::Timer()
{
    restart();
}

void Timer::restart()
{
    start = std::chrono::high_resolution_clock::now();
}

long long Timer::elapsedMs()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( elapsed );

    restart();
    return ms.count();
}
