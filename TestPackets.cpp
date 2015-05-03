
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

TEST(Packets, PacketWriteTest)
{
    PingPacket packet;
    std::ostringstream outputStream;
    std::string expectedOutput = "\xFF\x01\xFF";

    outputStream << packet;

    CHECK_EQUAL(expectedOutput, outputStream.str());
}

TEST(Packets, PacketParameterWriteTest)
{
    DigitalOutputPacket packet1(13, true);
    std::ostringstream outputStream;
    std::string expectedOutput;
    std::string actualOutput;

    expectedOutput = "\xFF\x02\x0D\x02\xFF";
    packet1.write(outputStream);
    actualOutput = outputStream.str();

    CHECK_EQUAL(expectedOutput, actualOutput);

    outputStream.str("");
    DigitalOutputPacket packet2(5, false);

    expectedOutput = "\xFF\x02\x05\x01\xFF";
    packet2.write(outputStream);
    actualOutput = outputStream.str();

    CHECK_EQUAL(expectedOutput, actualOutput);
}

TEST(Packets, PacketReadTest)
{
    std::istringstream inputStream;
    inputStream.str("\xFF\x01\xFF");
    PingPacket expectedPacket;

    Packet* packet = Packet::Read(inputStream);

    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
}

TEST(Packets, PacketParameterReadTest)
{
    std::istringstream inputStream;
    Packet* actualPacket;
    DigitalOutputPacket expectedPacket1(7, true);
    DigitalOutputPacket expectedPacket2(4, false);

    inputStream.str("\xFF\x02\x07\x02\xFF");
    actualPacket = Packet::Read(inputStream);

    CHECK(actualPacket != NULL);
    CHECK_EQUAL(expectedPacket1, *actualPacket);
    CHECK(expectedPacket2 != *actualPacket);

    delete actualPacket;

    inputStream.str("\xFF\x02\x04\x01\xFF");
    actualPacket = Packet::Read(inputStream);

    CHECK(actualPacket != NULL);
    CHECK_EQUAL(expectedPacket2, *actualPacket);
    CHECK(expectedPacket1 != *actualPacket);

    delete actualPacket;
}

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
