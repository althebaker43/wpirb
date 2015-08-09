
#include "AnalogInput.h"
#include <stdlib.h>


AnalogInput::AnalogInput(
        uint32_t channel
        ) :
    Input(channel)
{
}

AnalogInput::~AnalogInput()
{
}

uint16_t
AnalogInput::GetValue()
{
    return Get();
}

Packet*
AnalogInput::getNextPacket()
{
    return (Packet*)getNextPacketIfTimedOut();
}
