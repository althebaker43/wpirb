#ifndef TIMER_H
#define TIMER_H

#include <time.h>

/**
 * Class used to keep track of time
 */
class Timer
{
    public:

        /**
         * Default constructor
         */
        Timer();

        /**
         * Returns the amount of time elapsed since last started
         *
         * \return Number of seconds elapsed
         */
        double Get();

        /**
         * Resets the timer
         */
        void Reset();

        /**
         * Starts the timer
         */
        void Start();

        /**
         * Stops the timer
         */
        void Stop();

        /**
         * Indicates if the given amount of time has elapsed
         *
         * \return True if the time has elapse, false otherwise
         */
        bool HasPeriodPassed(
                double period
                );

    protected:

        /**
         * System time accessor type
         */
        typedef int (*TimeAccessor)(clockid_t, struct timespec*);

        /**
         * System time accessor
         */
        TimeAccessor getTime;

    private:

        /**
         * ID of the system clock to use for time keeping
         */
        static const clockid_t OUR_CLOCK_ID = CLOCK_MONOTONIC;

        /**
         * Provides the system time
         *
         * \return Number of seconds elapsed
         */
        double GetSystemTime();

        /**
         * Number of seconds elapsed
         */
        double myTime;

        /**
         * Time when timer was last started
         */
        double myStartTime;

        /**
         * Indicates if the timer is stopped
         */
        bool myIsStopped;
};

#endif /* ifndef TIMER_H */
