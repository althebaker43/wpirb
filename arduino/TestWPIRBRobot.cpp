
#include "WPIRBRobot.h"
#include "Arduino.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


TEST_GROUP(WPIRBRobot)
{
    void teardown()
    {
        mock().clear();
    }
};


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
