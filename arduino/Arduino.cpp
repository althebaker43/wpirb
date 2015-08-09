
#include "Arduino.h"
#include "CppUTestExt/MockSupport.h"


SerialHandler Serial;


void
SerialHandler::begin(
        unsigned int baud
        )
{
    mock().actualCall("begin").onObject(this).withParameter("baud", baud);
}

unsigned int
SerialHandler::available()
{
    return mock().actualCall("available").onObject(this).returnUnsignedIntValue();
}

byte
SerialHandler::read()
{
    return mock().actualCall("read").onObject(this).returnUnsignedIntValue();
}

void
SerialHandler::write(
        byte data
        )
{
    mock().actualCall("write").onObject(this).withParameter("data", data);
}

void
SerialHandler::flush()
{
    mock().actualCall("flush").onObject(this);
}

void
pinMode(
        unsigned int    pin,
        unsigned int    mode
       )
{
    mock().actualCall("pinMode").withParameter("pin", pin).withParameter("mode", mode);
}

void
digitalWrite(
        unsigned int    pin,
        unsigned int    value
        )
{
    mock().actualCall("digitalWrite").withParameter("pin", pin).withParameter("value", value);
}

unsigned int
digitalRead(
        unsigned int    pin
        )
{
    return mock().actualCall("digitalRead").withParameter("pin", pin).returnUnsignedIntValue();
}

unsigned int
analogRead(
        unsigned int    pin
        )
{
    return mock().actualCall("analogRead").withParameter("pin", pin).returnUnsignedIntValue();
}
