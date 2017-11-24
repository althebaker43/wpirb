
#include "Timer.h"

using namespace frc;


Timer::Timer() :
    getTime(clock_gettime),
    myTime(0.0),
    myStartTime(0.0),
    myIsStopped(true)
{
}

double
Timer::Get()
{
    if (myIsStopped == true)
    {
        return myTime;
    }
    else
    {
        return (GetSystemTime() - myStartTime + myTime);
    }
}

double
Timer::GetSystemTime()
{
    struct timespec currentTimeSpec;

    getTime(
            OUR_CLOCK_ID,
            &currentTimeSpec
           );

    double currentTime = currentTimeSpec.tv_sec + (1e-9 * currentTimeSpec.tv_nsec);

    return currentTime;
}

void
Timer::Reset()
{
    myTime = 0.0;

    if (myIsStopped == false)
    {
        myStartTime = GetSystemTime();
    }
}

void
Timer::Start()
{
    myIsStopped = false;
    myStartTime = GetSystemTime();
}

void
Timer::Stop()
{
    myTime = Get();
    myIsStopped = true;
}

bool
Timer::HasPeriodPassed(
        double period
        )
{
    return (Get() >= period);
}
