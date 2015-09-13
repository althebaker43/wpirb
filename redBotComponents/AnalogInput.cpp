
#include "AnalogInput.h"
#include <stdlib.h>


AnalogInputPacket::AnalogInputPacket() :
    RedBotPacket(TYPE_AINPUT, "AINPUT", BID_AINPUT),
    myPin(1)
{
}

AnalogInputPacket::AnalogInputPacket(
        unsigned int pin
        ) :
    RedBotPacket(TYPE_AINPUT, "AINPUT", BID_AINPUT),
    myPin(pin)
{
}

void
AnalogInputPacket::writeContents(
        std::ostream& outputStream
        ) const
{
    outputStream << (unsigned char)(myPin + 1);
}

void
AnalogInputPacket::getXMLElements(
        XMLElements& elements
        ) const
{
    elements.add(
            new XMLDataElement<unsigned int>(
                "pin",
                myPin
                )
            );
}

void
AnalogInputPacket::read(
        std::istream& inputStream
        )
{
    int pin = -1;

    pin = inputStream.get();
    myPin = (unsigned int)(pin - 1);
    if (inputStream.good() == false)
    {
        return;
    }

    inputStream.get();
}

bool
AnalogInputPacket::isValid() const
{
    return true;
}

bool
AnalogInputPacket::operator==(
        const Packet& packet
        ) const
{
    return true;
}

unsigned int
AnalogInputPacket::getPin() const
{
    return myPin;
}


AnalogValuePacket::AnalogValuePacket() :
    RedBotPacket(TYPE_AVALUE, "AVALUE", BID_AVALUE),
    myPin(1),
    myValue(0)
{
}

AnalogValuePacket::AnalogValuePacket(
        unsigned int    pin,
        unsigned int    value
        ) :
    RedBotPacket(TYPE_AVALUE, "AVALUE", BID_AVALUE),
    myPin(pin),
    myValue(value)
{
}

void
AnalogValuePacket::writeContents(
        std::ostream&   outputStream
        ) const
{
    outputStream
        << (unsigned char)(myPin + 1)
        << (unsigned char)(((0x3E0 & myValue) >> 5) + 1)
        << (unsigned char)((0x01F & myValue) + 1);
}

void
AnalogValuePacket::getXMLElements(
        XMLElements& elements
        ) const
{
    elements.add(
            new XMLDataElement<unsigned int>(
                "pin",
                myPin
                )
            );
    elements.add(
            new XMLDataElement<unsigned int>(
                "value",
                myValue
                )
            );
}

void
AnalogValuePacket::read(
        std::istream&   inputStream
        )
{
    int pin = -1;
    int valueHi = -1;
    int valueLo = -1;

    pin = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }
    myPin = (unsigned int)(pin - 1);

    valueHi = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }

    valueLo = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }

    myValue = (unsigned int)(((0x1F & (valueHi - 1)) << 5) | (0x1F & (valueLo - 1)));

    inputStream.get();
}

bool
AnalogValuePacket::isValid() const
{
    return true;
}

bool
AnalogValuePacket::operator==(
        const Packet& packet
        ) const
{
    return true;
}

unsigned int
AnalogValuePacket::getPin() const
{
    return myPin;
}

unsigned int
AnalogValuePacket::getValue() const
{
    return myValue;
}


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

bool
AnalogInput::processPacket(
        const Packet& packet
        )
{
    return processDataPacket(packet);
}
