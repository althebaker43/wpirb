
#include "RedBot.h"
#include "IOBuffer.h"
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
    fprintf(packetFile, "\xFF\x01\xFF");
    InputBuffer iBuffer(packetFile);

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
