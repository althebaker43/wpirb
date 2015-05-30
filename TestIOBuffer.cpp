
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
    InputFileBuffer iBuffer(packetFile);

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

    Packet* packet = Packet::Read(iBuffer.getInputStream());

    CHECK(packet != NULL);

    PingPacket expectedPacket;

    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
}

TEST(IOBuffer, InputBufferMultiReadTest)
{
    InputFileBuffer iBuffer(packetFile);
    PingPacket expectedPacket;
    Packet* packet = NULL;

    fprintf(packetFile, "\xFF\x01\xFF\xFF\x01\xFF");
    fflush(packetFile);
    rewind(packetFile);

    while (iBuffer.read() == true);

    CHECK(iBuffer.isPacketComplete());

    packet = Packet::Read(iBuffer.getInputStream());

    CHECK(packet != NULL);
    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
    iBuffer.clear();

    CHECK(iBuffer.isPacketComplete() == false);

    while (iBuffer.read() == true);

    CHECK(iBuffer.isPacketComplete());

    packet = Packet::Read(iBuffer.getInputStream());

    CHECK(packet != NULL);
    CHECK_EQUAL(expectedPacket, *packet);

    delete packet;
}

TEST(IOBuffer, InputBufferInvalidPacketTest)
{
    InputFileBuffer iBuffer(packetFile);
    Packet* packet = NULL;

    fprintf(packetFile, "\xFF\x81\x0A\xFF\xFF\x82\xFF");
    fflush(packetFile);
    rewind(packetFile);

    while (iBuffer.read() == true);

    CHECK(iBuffer.isPacketComplete());

    packet = Packet::Read(iBuffer.getInputStream());

    CHECK_EQUAL((Packet*)NULL, packet);

    iBuffer.clear();

    while (iBuffer.read() == true);

    CHECK(iBuffer.isPacketComplete());

    packet = Packet::Read(iBuffer.getInputStream());

    CHECK(NULL != packet);
    CHECK_EQUAL(Packet::TYPE_ACK, packet->getType());

    delete packet;
}

TEST(IOBuffer, InputBufferResyncTest)
{
    InputFileBuffer iBuffer(packetFile);
    Packet* packet = NULL;

    fprintf(packetFile, "\xFF\xFF\xFF\xFF\xFF");
    fflush(packetFile);
    rewind(packetFile);

    CHECK_FALSE(iBuffer.readPacket());

    rewind(packetFile);
    fprintf(packetFile, "\xFF\x82\xFF");
    fflush(packetFile);
    rewind(packetFile);

    CHECK(iBuffer.readPacket());
    
    packet = Packet::Read(iBuffer.getInputStream());

    CHECK(NULL != packet);
    CHECK_EQUAL(Packet::TYPE_ACK, packet->getType());

    delete packet;
    iBuffer.clear();

    rewind(packetFile);
    fprintf(packetFile, "\xFF\xFF\xFF\xFF\xFF");
    fflush(packetFile);
    rewind(packetFile);

    CHECK_FALSE(iBuffer.readPacket());
}

TEST(IOBuffer, OutputBufferWriteTest)
{
    OutputFileBuffer oBuffer(packetFile);
    PingPacket outputPacket;
    outputPacket.write(oBuffer.getOutputStream());

    CHECK(oBuffer.write());
    CHECK(oBuffer.isPacketComplete() == false);
    CHECK(oBuffer.write());
    CHECK(oBuffer.isPacketComplete() == false);
    CHECK(oBuffer.write());
    CHECK(oBuffer.isPacketComplete() == true);
    CHECK(oBuffer.write() == false);
    CHECK(oBuffer.isPacketComplete() == true);
    CHECK(oBuffer.write() == false);

    rewind(packetFile);

    char writtenPacket [4];

    CHECK(fgets(writtenPacket, 4, packetFile) != NULL);
    STRCMP_EQUAL("\xFF\x01\xFF", writtenPacket);
}

TEST(IOBuffer, OutputBufferMultiWriteTest)
{
    OutputFileBuffer oBuffer(packetFile);
    PingPacket outputPacket;
    char writtenPacket [4];

    outputPacket.write(oBuffer.getOutputStream());

    CHECK(oBuffer.isPacketComplete() == false);

    while (oBuffer.write() == true);

    CHECK(oBuffer.isPacketComplete());
    CHECK(oBuffer.write() == false);

    rewind(packetFile);

    CHECK(fgets(writtenPacket, 4, packetFile) != NULL);
    STRCMP_EQUAL("\xFF\x01\xFF", writtenPacket);

    oBuffer.clear();
    outputPacket.write(oBuffer.getOutputStream());

    CHECK(oBuffer.isPacketComplete() == false);

    while (oBuffer.write() == true);

    CHECK(oBuffer.isPacketComplete());

    rewind(packetFile);

    CHECK(fgets(writtenPacket, 4, packetFile) != NULL);
    STRCMP_EQUAL("\xFF\x01\xFF", writtenPacket);
}

TEST(IOBuffer, IOBufferPacketIntegrityTest)
{
    OutputFileBuffer oBuffer(packetFile);
    InputFileBuffer iBuffer(packetFile);
    PingPacket outputPacket;

    outputPacket.write(oBuffer.getOutputStream());
    while (oBuffer.write() == true);

    rewind(packetFile);

    while (iBuffer.read() == true);
    Packet* inputPacket = Packet::Read(iBuffer.getInputStream());

    CHECK(inputPacket != NULL);
    CHECK_EQUAL(outputPacket, *inputPacket);

    delete inputPacket;
}
