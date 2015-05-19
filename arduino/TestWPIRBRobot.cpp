
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
    mock().expectOneCall("available").onObject(&Serial).andReturnValue(0);

    robot.setup();
    robot.loop();

    mock().checkExpectations();
}
