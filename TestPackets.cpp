
#include "Packet.h"
#include "CppUTest/TestHarness.h"
#include <sstream>
#include <list>


TEST_GROUP(Packets)
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

TEST(Packets, DigitalInputPacket)
{
    DigitalInputPacket dInPacket1(3);
    std::ostringstream outputStream;

    CHECK_EQUAL(3, dInPacket1.getPin());

    dInPacket1.write(outputStream);

    STRCMP_EQUAL("\xFF\x03\x03\xFF", outputStream.str().c_str());

    DigitalInputPacket dInPacket2(12);

    CHECK_EQUAL(12, dInPacket2.getPin());

    outputStream.str("");
    dInPacket2.write(outputStream);

    STRCMP_EQUAL("\xFF\x03\x0C\xFF", outputStream.str().c_str());
}

TEST(Packets, DigitalValuePacket)
{
    DigitalValuePacket dValPacket1(2, true);

    CHECK_EQUAL(Packet::TYPE_DVALUE, dValPacket1.getType());
    CHECK_EQUAL(2, dValPacket1.getPin());
    CHECK_EQUAL(true, dValPacket1.getValue());

    std::istringstream inputStream;
    inputStream.str("\xFF\x81\x0A\x01\xFF");

    Packet* packet2 = Packet::Read(inputStream);
    myPackets.push_back(packet2);

    CHECK(packet2 != NULL);
    CHECK_EQUAL(Packet::TYPE_DVALUE, packet2->getType());

    DigitalValuePacket* dValPacket2 = static_cast<DigitalValuePacket*>(packet2);

    CHECK_EQUAL(10, dValPacket2->getPin());
    CHECK_EQUAL(false, dValPacket2->getValue());

    inputStream.str("\xFF\x81\x0F\x02\xFF");

    Packet* packet3 = Packet::Read(inputStream);
    myPackets.push_back(packet3);

    CHECK(packet3 != NULL);
    CHECK_EQUAL(Packet::TYPE_DVALUE, packet2->getType());

    DigitalValuePacket* dValPacket3 = static_cast<DigitalValuePacket*>(packet3);

    CHECK_EQUAL(15, dValPacket3->getPin());
    CHECK_EQUAL(true, dValPacket3->getValue());
}
