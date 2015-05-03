
#include "IOBuffer.h"
#include "Packet.h"
#include "CppUTest/TestHarness.h"


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
