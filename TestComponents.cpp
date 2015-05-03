
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "Packet.h"
#include "RedBot.h"
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

        RedBot::ClearRegisteredComponents();
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

    dOut.Set(0);
    dOut.Set(1);
    Packet* packet2 = dOut.getNextPacket();
    myPackets.push_back(packet2);

    CHECK(packet2 != NULL);
    CHECK_EQUAL(Packet::TYPE_DOUTPUT, packet2->getType());

    DigitalOutputPacket* dOutPacket2 = static_cast<DigitalOutputPacket*>(packet2);

    CHECK_EQUAL(5, dOutPacket2->getPin());
    CHECK_EQUAL(true, dOutPacket2->getValue());
}

TEST(Components, DigitalInputTest)
{
    DigitalInput dIn(4);

    Packet* packet1 = dIn.getNextPacket();
    myPackets.push_back(packet1);

    CHECK(packet1 != NULL);
    CHECK_EQUAL(Packet::TYPE_DINPUT, packet1->getType());

    DigitalInputPacket* dInPacket1 = static_cast<DigitalInputPacket*>(packet1);

    CHECK_EQUAL(4, dInPacket1->getPin());

    DigitalValuePacket dValPacket1(4, false);

    CHECK(dIn.processPacket(dValPacket1));
    CHECK_FALSE(dIn.Get());

    DigitalValuePacket dValPacket2(4, true);

    CHECK(dIn.processPacket(dValPacket2));
    CHECK_TRUE(dIn.Get());
}
