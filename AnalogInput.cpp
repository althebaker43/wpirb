
#include "AnalogInput.h"
#include <stdlib.h>


AnalogInputPacket::AnalogInputPacket() :
    myPin(1)
{
}

AnalogInputPacket::AnalogInputPacket(
        unsigned int pin
        ) :
    myPin(pin)
{
}

void
AnalogInputPacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream
        << '\xFF'
        << (unsigned char)BID_AINPUT
        << (unsigned char)(myPin + 1)
        << '\xFF';
}

void
AnalogInputPacket::writeXML(
        std::ostream& outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>AINPUT</type>"
        << "<pin>" << myPin << "</pin>"
        << "</packet>";
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

RedBotPacket::Type
AnalogInputPacket::getType() const
{
    return TYPE_AINPUT;
}

unsigned int
AnalogInputPacket::getPin() const
{
    return myPin;
}


AnalogValuePacket::AnalogValuePacket() :
    myPin(1),
    myValue(0)
{
}

AnalogValuePacket::AnalogValuePacket(
        unsigned int    pin,
        unsigned int    value
        ) :
    myPin(pin),
    myValue(value)
{
}

void
AnalogValuePacket::write(
        std::ostream&   outputStream
        ) const
{
    outputStream
        << '\xFF'
        << (unsigned char)BID_AVALUE
        << (unsigned char)(myPin + 1)
        << (unsigned char)(((0x3E0 & myValue) >> 5) + 1)
        << (unsigned char)((0x01F & myValue) + 1)
        << '\xFF';
}

void
AnalogValuePacket::writeXML(
        std::ostream&   outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>AVALUE</type>"
        << "<pin>" << myPin << "</pin>"
        << "<value>" << myValue << "</value>"
        << "</packet>";
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

RedBotPacket::Type
AnalogValuePacket::getType() const
{
    return TYPE_AVALUE;
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
