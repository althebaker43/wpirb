
#include "WPIRBRobot.h"
#include "Arduino.h"
#include "RedBotComponent.h"
#include "PinConfigPacket.h"
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
        byte = packetStream.get();

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
        byte = packetStream.get();

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

TEST(WPIRBRobot, DigitalInputTest)
{
    WPIRBRobot robot;
    
    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    mock().expectOneCall("digitalRead").withParameter("pin", 12).andReturnValue(LOW);
    SendPacket(
            DigitalInputPacket(12),
            DigitalValuePacket(
                12,
                false
                ),
            robot
            );

    mock().checkExpectations();

    mock().expectOneCall("digitalRead").withParameter("pin", 5).andReturnValue(HIGH);
    SendPacket(
            DigitalInputPacket(5),
            DigitalValuePacket(
                5,
                true
                ),
            robot
            );

    mock().checkExpectations();
}

TEST(WPIRBRobot, AnalogInputTest)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    mock().expectOneCall("analogRead").withParameter("pin", 3).andReturnValue(32);
    SendPacket(
            AnalogInputPacket(3),
            AnalogValuePacket(
                3,
                32
                ),
            robot
            );

    mock().checkExpectations();

    SendPacket(
            AnalogInputPacket(8),
            AcknowledgePacket(),
            robot
            );

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

    mock().expectOneCall("leftMotor").withParameter("speed", 75);
    SendPacket(
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                75,
                MotorDrivePacket::DIR_BACKWARD
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();

    mock().expectOneCall("leftMotor").withParameter("speed", 78);

    SendPacket(
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                78,
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

TEST(WPIRBRobot, MotorDriveFilterTest)
{
    WPIRBRobot robot;
    unsigned int speedThreshold = 64;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    // Zero speed
    mock().expectOneCall("rightMotor").withParameter("speed", 0);
    SendPacket(
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                0,
                MotorDrivePacket::DIR_FORWARD
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();

    // Very low forward speed
    mock().expectOneCall("leftMotor").withParameter("speed", 0);
    SendPacket(
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                speedThreshold,
                MotorDrivePacket::DIR_FORWARD
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();

    // Adequate speed
    mock().expectOneCall("rightMotor").withParameter("speed", speedThreshold+1);
    SendPacket(
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                speedThreshold+1,
                MotorDrivePacket::DIR_FORWARD
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();
}

TEST(WPIRBRobot, PinConfigTest)
{
    WPIRBRobot robot;

    mock().expectOneCall("begin").onObject(&Serial).withParameter("baud", 9600);
    robot.setup();
    mock().checkExpectations();

    mock().expectOneCall("pinMode").withParameter("pin", 8).withParameter("mode", INPUT);
    SendPacket(
            PinConfigPacket(
                8,
                PinConfigPacket::DIR_INPUT
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();

    mock().expectOneCall("pinMode").withParameter("pin", 12).withParameter("mode", OUTPUT);
    SendPacket(
            PinConfigPacket(
                12,
                PinConfigPacket::DIR_OUTPUT
                ),
            AcknowledgePacket(),
            robot
            );

    mock().checkExpectations();
}
