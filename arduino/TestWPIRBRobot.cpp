
#include "WPIRBRobot.h"
#include "Arduino.h"
#include "Packet.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <sstream>


TEST_GROUP(WPIRBRobot)
{
    void teardown()
    {
        mock().clear();
    }
};


void
SendPacket(
        const Packet&   requestPacket,
        const Packet&   responsePacket,
        WPIRBRobot&     robot
        )
{
    std::stringstream packetStream;

    packetStream << requestPacket;
    size_t packetLength = 0;

    while (true)
    {
        unsigned char byte;
        packetStream >> byte;

        if (packetStream.good() != true)
        {
            break;
        }

        mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
        mock().expectOneCall("read").onObject(&Serial).andReturnValue((unsigned int)byte);
        ++packetLength;
    }

    packetStream.clear();
    packetStream << responsePacket;

    while (true)
    {
        unsigned char byte;
        packetStream >> byte;

        if (packetStream.good() != true)
        {
            break;
        }

        mock().expectOneCall("write").onObject(&Serial).withParameter("data", (unsigned int)byte);
    }

    mock().expectOneCall("flush").onObject(&Serial);

    for(
            size_t byteIdx = 0;
            byteIdx < packetLength;
            ++byteIdx
       )
    {
        robot.loop();
    }
}


TEST(WPIRBRobot, BasicTest)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    mock().expectOneCall("available").onObject(&Serial).andReturnValue(0);
    robot.loop();
    mock().checkExpectations();

    mock().expectOneCall("available").onObject(&Serial).andReturnValue(0);
    robot.loop();
    mock().checkExpectations();

    mock().expectOneCall("available").onObject(&Serial).andReturnValue(0);
    robot.loop();
    mock().checkExpectations();
}

TEST(WPIRBRobot, PingTest)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    // First ping
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x01);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);

    // First acknowledge
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0x82);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("flush").onObject(&Serial);

    robot.loop();
    robot.loop();
    robot.loop();
    mock().checkExpectations();

    // Second ping
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x01);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);

    // Second acknowledge
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0x82);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("flush").onObject(&Serial);

    robot.loop();
    robot.loop();
    robot.loop();
    mock().checkExpectations();
}

TEST(WPIRBRobot, DigitalWriteTest)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);

    // Digital write
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x02);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x05);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x02);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);

    // Pin output
    mock().expectOneCall("pinMode").withParameter("pin", 5).withParameter("mode", OUTPUT);
    mock().expectOneCall("digitalWrite").withParameter("pin", 5).withParameter("value", HIGH);

    // First acknowledge
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0x82);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("flush").onObject(&Serial);

    // Ping
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x01);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);

    // Second acknowledge
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0x82);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("flush").onObject(&Serial);

    robot.setup();

    robot.loop();
    robot.loop();
    robot.loop();
    robot.loop();
    robot.loop();

    robot.loop();
    robot.loop();
    robot.loop();

    mock().checkExpectations();
}

TEST(WPIRBRobot, IncompletePacket)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    // Incomplete ping
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x01);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);

    robot.loop();
    robot.loop();
    mock().checkExpectations();

    // Second ping
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    robot.loop();
    mock().checkExpectations();
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x01);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);

    // Acknowledge
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0x82);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("flush").onObject(&Serial);

    robot.loop();
    robot.loop();
    mock().checkExpectations();
}

TEST(WPIRBRobot, OversizedPacket)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    robot.loop();
    mock().checkExpectations();

    for (int i = 0; i < 9; i ++ )
    {
        mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
        mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x01);
        robot.loop();
        mock().checkExpectations();
    }

    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    robot.loop();
    mock().checkExpectations();

    // Ping
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x01);
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);

    // First acknowledge
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0x82);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("flush").onObject(&Serial);

    robot.loop();
    robot.loop();
    robot.loop();

    mock().checkExpectations();
}

TEST(WPIRBRobot, MotorDrivePacketTest)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    mock().expectOneCall("rightMotor").withParameter("speed", 100);

    SendPacket(
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                100,
                MotorDrivePacket::DIR_FORWARD
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();

    mock().expectOneCall("leftMotor").withParameter("speed", 50);

    SendPacket(
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                50,
                MotorDrivePacket::DIR_BACKWARD
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();
}

TEST(WPIRBRobot, ResyncTest)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    // Fill buffer with bogus data
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
    mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    for (int i = 0; i < 8; i++)
    {
        mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
        mock().expectOneCall("read").onObject(&Serial).andReturnValue(0x0A);
    }

    // Resync
    for (int i = 0; i < 5; i++)
    {
        mock().expectOneCall("available").onObject(&Serial).andReturnValue(1);
        mock().expectOneCall("read").onObject(&Serial).andReturnValue(0xFF);
    }

    // Get acknowledge
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0x82);
    mock().expectOneCall("write").onObject(&Serial).withParameter("data", 0xFF);
    mock().expectOneCall("flush").onObject(&Serial);

    for (int i = 0; i < 14; i++)
    {
        robot.loop();
    }

    mock().checkExpectations();

    // Ping and acknowledge
    SendPacket(
            PingPacket(),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();
}
