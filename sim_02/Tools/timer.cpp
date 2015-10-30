#include "timer.h"

//Start the clock at the first definition of Timer
std::chrono::time_point<CLOCK> Timer::start = CLOCK::now();

/**
 * @brief      Returns the millisecond float value of the time between now and
 * when Timer was defined.
 *
 * @return     A float value that represents the change in time in milliseconds.
 */
float Timer::msDT()
{
    return (CLOCK::now() - start).count() / 1000000000.0f;
}