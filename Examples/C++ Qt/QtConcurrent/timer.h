#ifndef TIMER_H
#define TIMER_H


#include <chrono>


class Timer
{
    public:

                            Timer();

        void                restart();
        long long           elapsedMs();

    private:

        std::chrono::high_resolution_clock::time_point start;


}; // Timer


#endif // TIMER_H
