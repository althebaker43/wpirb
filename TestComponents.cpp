
#include "DigitalOutput.h"
#include "Packet.h"
#include "CppUTest/TestHarness.h"
#include <list>


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
    }
};

TEST(Components, DigitalOutputTest)
{
    DigitalOutput dOut(5);
    dOut.Set(1);
    Packet* packet1 = dOut.getNextPacket();
    myPackets.push_back(packet1);

    CHECK(packet1 != NULL);
    CHECK_EQUAL(Packet::TYPE_DOUTPUT, packet1->getType());

    DigitalOutputPacket* dOutPacket1 = static_cast<DigitalOutputPacket*>(packet1);

    CHECK_EQUAL(5, dOutPacket1->getPin());
    CHECK_EQUAL(true, dOutPacket1->getValue());

    POINTERS_EQUAL(NULL, dOut.getNextPacket());
}
