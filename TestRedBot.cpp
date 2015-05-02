
#include "RedBot.h"
#include "IOBuffer.h"
#include "Packet.h"
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

TEST(RedBot, PacketParameterWriteTest)
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

TEST(RedBot, PacketReadTest)
{
    std::istringstream inputStream;
    inputStream.str("\xFF\x01\xFF");
    PingPacket expectedPacket;

    Packet* packet = Packet::Read(inputStream);

    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
}

TEST(RedBot, PacketParameterReadTest)
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


TEST_GROUP(IOBuffer)
{
    FILE* packetFile;

    void setup()
    {
        packetFile = tmpfile();
    }

    void teardown()
    {
        fclose(packetFile);
    }
};

TEST(IOBuffer, InputBufferReadTest)
{
    InputBuffer iBuffer(packetFile);

    fprintf(packetFile, "\xFF\x01\xFF");
    fflush(packetFile);
    rewind(packetFile);

    CHECK(iBuffer.read());
    CHECK(iBuffer.isPacketComplete() == false);
    CHECK(iBuffer.read());
    CHECK(iBuffer.isPacketComplete() == false);
    CHECK(iBuffer.read());
    CHECK(iBuffer.isPacketComplete() == true);
    CHECK(iBuffer.read() == false);
    CHECK(iBuffer.isPacketComplete() == true);
    CHECK(iBuffer.read() == false);

    Packet* packet = Packet::Read(iBuffer.getContents());

    CHECK(packet != NULL);

    PingPacket expectedPacket;

    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
}

TEST(IOBuffer, InputBufferMultiReadTest)
{
    InputBuffer iBuffer(packetFile);
    PingPacket expectedPacket;
    Packet* packet = NULL;

    fprintf(packetFile, "\xFF\x01\xFF\xFF\x01\xFF");
    fflush(packetFile);
    rewind(packetFile);

    while (iBuffer.read() == true);

    CHECK(iBuffer.isPacketComplete());

    packet = Packet::Read(iBuffer.getContents());

    CHECK(packet != NULL);
    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
    iBuffer.clear();

    CHECK(iBuffer.isPacketComplete() == false);

    while (iBuffer.read() == true);

    CHECK(iBuffer.isPacketComplete());

    packet = Packet::Read(iBuffer.getContents());

    CHECK(packet != NULL);
    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
}

TEST(IOBuffer, OutputBufferWriteTest)
{
    OutputBuffer oBuffer(packetFile);
    PingPacket outputPacket;
    outputPacket.write(oBuffer.getContents());

    CHECK(oBuffer.write());
    CHECK(oBuffer.isPacketComplete() == false);
    CHECK(oBuffer.write());
    CHECK(oBuffer.isPacketComplete() == false);
    CHECK(oBuffer.write());
    CHECK(oBuffer.isPacketComplete() == true);
    CHECK(oBuffer.write() == false);
    CHECK(oBuffer.isPacketComplete() == true);
    CHECK(oBuffer.write() == false);

    fflush(packetFile);
    rewind(packetFile);

    char writtenPacket [4];

    CHECK(fgets(writtenPacket, 4, packetFile) != NULL);
    STRCMP_EQUAL("\xFF\x01\xFF", writtenPacket);
}

TEST(IOBuffer, OutputBufferMultiWriteTest)
{
    OutputBuffer oBuffer(packetFile);
    PingPacket outputPacket;
    char writtenPacket [4];

    outputPacket.write(oBuffer.getContents());

    CHECK(oBuffer.isPacketComplete() == false);

    while (oBuffer.write() == true);

    CHECK(oBuffer.isPacketComplete());
    CHECK(oBuffer.write() == false);

    fflush(packetFile);
    rewind(packetFile);

    CHECK(fgets(writtenPacket, 4, packetFile) != NULL);
    STRCMP_EQUAL("\xFF\x01\xFF", writtenPacket);

    oBuffer.clear();
    outputPacket.write(oBuffer.getContents());

    CHECK(oBuffer.isPacketComplete() == false);

    while (oBuffer.write() == true);

    CHECK(oBuffer.isPacketComplete());

    fflush(packetFile);
    rewind(packetFile);

    CHECK(fgets(writtenPacket, 4, packetFile) != NULL);
    STRCMP_EQUAL("\xFF\x01\xFF", writtenPacket);
}

TEST(IOBuffer, IOBufferPacketIntegrityTest)
{
    OutputBuffer oBuffer(packetFile);
    InputBuffer iBuffer(packetFile);
    PingPacket outputPacket;

    outputPacket.write(oBuffer.getContents());
    while (oBuffer.write() == true);

    fflush(packetFile);
    rewind(packetFile);

    while (iBuffer.read() == true);
    Packet* inputPacket = Packet::Read(iBuffer.getContents());

    CHECK(inputPacket != NULL);
    CHECK_EQUAL(outputPacket, *inputPacket);

    delete inputPacket;
}
