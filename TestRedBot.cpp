
#include "RedBot.h"
#include "CppUTest/TestHarness.h"
#include <fstream>
#include <sstream>
#include <stdio.h>


TEST_GROUP(RedBot)
{
};

TEST(RedBot, PacketWriteTest)
{
    PingPacket packet;
    std::ostringstream outputStream;
    std::string expectedOutput = "\xFF\x01\xFF";

    outputStream << packet;

    CHECK_EQUAL(expectedOutput, outputStream.str());
}

TEST(RedBot, PacketReadTest)
{
    std::istringstream inputStream;
    inputStream.str("\xFF\x01\xFF");
    PingPacket expectedPacket;

    Packet* packet = Packet::Read(inputStream);

    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
}
