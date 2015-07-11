
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
    std::istringstream inputStream;

    CHECK_EQUAL(3, dInPacket1.getPin());

    dInPacket1.write(outputStream);

    STRCMP_EQUAL("\xFF\x03\x03\xFF", outputStream.str().c_str());

    DigitalInputPacket dInPacket2(12);

    CHECK_EQUAL(12, dInPacket2.getPin());

    outputStream.str("");
    dInPacket2.write(outputStream);

    STRCMP_EQUAL("\xFF\x03\x0C\xFF", outputStream.str().c_str());

    inputStream.str("\xFF\x03\x01\xFF");
    Packet* packet3 = Packet::Read(inputStream);

    CHECK(NULL != packet3);
    CHECK_EQUAL(Packet::TYPE_DINPUT, packet3->getType());

    DigitalInputPacket* dInPacket3 = static_cast<DigitalInputPacket*>(packet3);

    CHECK_EQUAL(1, dInPacket3->getPin());

    delete dInPacket3;
}

TEST(Packets, MotorDrivePacket)
{
    std::ostringstream outputStream;
    std::istringstream inputStream;

    MotorDrivePacket mDrivePacket1(
            MotorDrivePacket::MOTOR_RIGHT,
            255,
            MotorDrivePacket::DIR_FORWARD
            );
    mDrivePacket1.write(outputStream);

    STRCMP_EQUAL("\xFF\x05\x01\x01\x10\x10\xFF", outputStream.str().c_str());

    outputStream.str("");
    MotorDrivePacket mDrivePacket2(
            MotorDrivePacket::MOTOR_LEFT,
            128,
            MotorDrivePacket::DIR_BACKWARD
            );
    mDrivePacket2.write(outputStream);

    STRCMP_EQUAL("\xFF\x05\x02\x02\x09\x01\xFF", outputStream.str().c_str());

    inputStream.str("\xFF\x05\x02\x01\x01\x01\xFF");
    Packet* packet3 = Packet::Read(inputStream);

    CHECK(NULL != packet3);
    CHECK_EQUAL(Packet::TYPE_MDRIVE, packet3->getType());

    MotorDrivePacket* mDrivePacket3 = static_cast<MotorDrivePacket*>(packet3);

    CHECK_EQUAL(MotorDrivePacket::MOTOR_LEFT, mDrivePacket3->getMotor());
    CHECK_EQUAL(0, mDrivePacket3->getSpeed());
    CHECK_EQUAL(MotorDrivePacket::DIR_FORWARD, mDrivePacket3->getDirection());

    delete mDrivePacket3;
}

TEST(Packets, DigitalValuePacket)
{
    DigitalValuePacket dValPacket1(2, true);

    CHECK_EQUAL(Packet::TYPE_DVALUE, dValPacket1.getType());
    CHECK_EQUAL(2, dValPacket1.getPin());
    CHECK_EQUAL(true, dValPacket1.getValue());

    std::ostringstream outputStream;
    outputStream << dValPacket1;

    STRCMP_EQUAL("\xFF\x81\x02\x02\xFF", outputStream.str().c_str());

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

TEST(Packets, AcknowledgePacket)
{
    AcknowledgePacket ackPacket1;
    std::stringstream packetStream;

    packetStream << ackPacket1;

    STRCMP_EQUAL("\xFF\x82\xFF", packetStream.str().c_str());

    Packet* packet2 = Packet::Read(packetStream);

    CHECK(NULL != packet2);
    CHECK_EQUAL(Packet::TYPE_ACK, packet2->getType());

    delete packet2;
}

TEST(Packets, DigitalOutputPacketXML)
{
    std::ostringstream packetStream;
    std::string expectedOutput;
    DigitalOutputPacket dOutPacket1(7, true);

    dOutPacket1.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>DOUTPUT</type>"
        "<pin>7</pin>"
        "<value>1</value>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());

    DigitalOutputPacket dOutPacket2(8, false);

    packetStream.str("");
    dOutPacket2.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>DOUTPUT</type>"
        "<pin>8</pin>"
        "<value>0</value>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());
}

TEST(Packets, DigitalInputPacketXML)
{
    std::ostringstream packetStream;
    std::string expectedOutput;
    DigitalInputPacket dInPacket1(13);

    dInPacket1.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>DINPUT</type>"
        "<pin>13</pin>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());

    DigitalInputPacket dInPacket2(4);

    packetStream.str("");
    dInPacket2.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>DINPUT</type>"
        "<pin>4</pin>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());
}

TEST(Packets, MotorDrivePacketXML)
{
    std::ostringstream packetStream;
    std::string expectedOutput;

    MotorDrivePacket mDrivePacket1(
            MotorDrivePacket::MOTOR_RIGHT,
            255,
            MotorDrivePacket::DIR_BACKWARD
            );
    mDrivePacket1.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>MDRIVE</type>"
        "<motor>right</motor>"
        "<speed>255</speed>"
        "<direction>backward</direction>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());

    packetStream.str("");
    MotorDrivePacket mDrivePacket2(
            MotorDrivePacket::MOTOR_LEFT,
            34,
            MotorDrivePacket::DIR_FORWARD
            );
    mDrivePacket2.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>MDRIVE</type>"
        "<motor>left</motor>"
        "<speed>34</speed>"
        "<direction>forward</direction>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());
}

TEST(Packets, DigitalValuePacketXML)
{
    std::ostringstream packetStream;
    std::string expectedOutput;
    DigitalValuePacket dValPacket1(10, true);

    dValPacket1.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>DVALUE</type>"
        "<pin>10</pin>"
        "<value>1</value>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());

    DigitalValuePacket dValPacket2(6, false);

    packetStream.str("");
    dValPacket2.writeXML(packetStream);

    expectedOutput =
        "<packet>"
        "<type>DVALUE</type>"
        "<pin>6</pin>"
        "<value>0</value>"
        "</packet>";
    CHECK_EQUAL(expectedOutput, packetStream.str());
}

TEST(Packets, InvalidReadPacket)
{
    std::istringstream inputStream;
    std::ostringstream outputStream;

    DigitalValuePacket dValPacket;

    outputStream << dValPacket;

    STRCMP_EQUAL("\xFF\x81", outputStream.str().c_str());

    inputStream.str("\x0A\xFF");
    inputStream >> dValPacket;

    CHECK_FALSE(dValPacket.isValid());

    outputStream.str("");
    outputStream << dValPacket;
    std::string dValPacketData = outputStream.str();

    STRCMP_EQUAL("\xFF\x81\x0A\xFF", dValPacketData.c_str());
}
