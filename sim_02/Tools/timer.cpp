#include "timer.h"

std::chrono::time_point<CLOCK> Timer::start = CLOCK::now();

Timer::Timer(){}

float Timer::msDT()
{
    return (CLOCK::now() - start).count() / 1000000000.0f;
}