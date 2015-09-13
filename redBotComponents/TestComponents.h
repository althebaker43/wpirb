#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H

#include "Component.h"
#include "Packet.h"
#include "CppUTest/TestHarness.h"
#include <list>

TEST_GROUP(Components)
{
    std::list<Packet*> myPackets;

    void teardown()
    {
        Component::ClearRegisteredComponents();

        for(
                std::list<Packet*>::iterator packetIter = myPackets.begin();
                packetIter != myPackets.end();
                ++packetIter
           )
        {
            delete (*packetIter);
        }

        myPackets.clear();
    }
};

#endif /* ifndef TESTCOMPONENT_H */
