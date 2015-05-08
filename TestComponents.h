#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H

#include "Timer.h"
#include "Packet.h"
#include "RedBot.h"
#include "CppUTest/TestHarness.h"
#include <list>

struct timespec MockTime;

int
MockTimeAccessor(
        clockid_t           clock,
        struct timespec*    time
        )
{
    time->tv_sec = MockTime.tv_sec;
    time->tv_nsec = MockTime.tv_nsec;
    return 0;
}

TEST_GROUP(Components)
{
    std::list<Packet*> myPackets;

    void teardown()
    {
        for(
                std::list<Packet*>::iterator packetIter = myPackets.begin();
                packetIter != myPackets.end();
                ++packetIter
           )
        {
            delete (*packetIter);
        }

        myPackets.clear();

        RedBot::ClearRegisteredComponents();
    }
};

class MockTimer : public Timer
{
    public:

        MockTimer(
                TimeAccessor accessor
                ) :
            Timer()
        {
            Timer::getTime = accessor;
        }
};

#endif /* ifndef TESTCOMPONENT_H */
