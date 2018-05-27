
#include "RedBot.h"
#include "CppUTestExt/MockSupport.h"

using namespace rb;


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


RedBotEncoder::RedBotEncoder(int leftPin, int rightPin)
{
}

long
RedBotEncoder::getTicks(WHEEL wheel)
{
  return mock().actualCall("encoderGetTicks").withParameter("motor", wheel).returnIntValue();
}

void
RedBotEncoder::clearEnc(WHEEL wheel)
{
  mock().actualCall("encoderClear").withParameter("motor", wheel);
}
