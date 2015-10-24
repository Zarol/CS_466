#ifndef TIMER_H
#define TIMER_H

#include <chrono>

typedef std::chrono::high_resolution_clock CLOCK; 

class Timer
{
public:
    static float msDT();

private:
    Timer();
    static std::chrono::time_point<CLOCK> start;
};

#endif  //  TIMER_H