
#include "RedBot.h"
#include "CppUTestExt/MockSupport.h"


RedBotMotors::RedBotMotors()
{
}

void
RedBotMotors::rightMotor(
        int speed
        )
{
    mock().actualCall("rightMotor").withParameter("speed", speed);
}

void
RedBotMotors::leftMotor(
        int speed
        )
{
    mock().actualCall("leftMotor").withParameter("speed", speed);
}
