
#include "DigitalInput.h"
#include "Packet.h"
#include <stdlib.h>


DigitalInput::DigitalInput(
        uint32_t channel
        ) :
    myChannel(channel),
    myValue(false)
{
}

DigitalInput::~DigitalInput()
{
}

bool
DigitalInput::Get()
{
    return myValue;
}

Packet*
DigitalInput::getNextPacket()
{
    return new DigitalInputPacket(myChannel);
}

bool
DigitalInput::processPacket(
        const Packet& packet
        )
{
    const DigitalValuePacket& dValPacket = static_cast<const DigitalValuePacket&>(packet);
    myValue = dValPacket.getValue();

    return true;
}
