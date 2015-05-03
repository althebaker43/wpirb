
#include "DigitalOutput.h"
#include "Packet.h"
#include <stdlib.h>


DigitalOutput::DigitalOutput(
        uint32_t channel
        ) :
    myChannel(channel),
    myCurrentPacket(NULL)
{
}

DigitalOutput::~DigitalOutput()
{
}

void
DigitalOutput::Set(
        uint32_t value
        )
{
    myCurrentPacket = new DigitalOutputPacket(
            myChannel,
            ((value == 0) ? false : true)
            );
}

Packet*
DigitalOutput::getNextPacket()
{
    Packet* packet = myCurrentPacket;
    myCurrentPacket = NULL;

    return packet;
}
