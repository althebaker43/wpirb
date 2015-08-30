
#include "PinConfigPacket.h"


PinConfigPacket::PinConfigPacket() :
    myPin(1),
    myDirection(DIR_INPUT)
{
}

PinConfigPacket::PinConfigPacket(
        unsigned int                pin,
        RedBotPacket::PinDirection  dir
        ) :
    myPin(pin),
    myDirection(dir)
{
}

void
PinConfigPacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream
        << '\xFF'
        << (unsigned char)BID_PINCONFIG
        << (unsigned char)myPin
        << (unsigned char)myDirection
        << '\xFF';
}

void
PinConfigPacket::writeXML(
        std::ostream& outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>PINCONFIG</type>"
        << "<pin>" << myPin << "</pin>"
        << "<direction>" << ((myDirection == DIR_OUTPUT) ? "OUTPUT" : "INPUT") << "</direction>"
        << "</packet>";
}

void
PinConfigPacket::read(
        std::istream& inputStream
        )
{
    myPin = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }

    switch (inputStream.get())
    {
        case 1:
            myDirection = DIR_OUTPUT;
            break;

        case 2:
            myDirection = DIR_INPUT;
            break;

        default:
            break;
    };
    if (inputStream.good() == false)
    {
        return;
    }

    inputStream.get();
}

bool
PinConfigPacket::isValid() const
{
    return true;
}

bool
PinConfigPacket::operator==(
        const Packet& packet
        ) const
{
    return true;
}

RedBotPacket::Type
PinConfigPacket::getType() const
{
    return TYPE_PINCONFIG;
}

unsigned int
PinConfigPacket::getPin() const
{
    return myPin;
}

RedBotPacket::PinDirection
PinConfigPacket::getDirection() const
{
    return myDirection;
}


PinConfigInfoPacket::PinConfigInfoPacket() :
    myPin(1),
    myDirection(DIR_INPUT)
{
}

PinConfigInfoPacket::PinConfigInfoPacket(
        unsigned int                pin,
        RedBotPacket::PinDirection  dir
        ) :
    myPin(pin),
    myDirection(dir)
{
}

void
PinConfigInfoPacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream
        << '\xFF'
        << (unsigned char)BID_PINCONFIGINFO
        << (unsigned char)myPin
        << (unsigned char)myDirection
        << '\xFF';
}

void
PinConfigInfoPacket::writeXML(
        std::ostream& outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>PINCONFIGINFO</type>"
        << "<pin>" << myPin << "</pin>"
        << "<direction>"
        << ((myDirection == DIR_INPUT) ? "INPUT" : "OUTPUT")
        << "</direction>"
        << "</packet>";
}

void
PinConfigInfoPacket::read(
        std::istream& inputStream
        )
{
    int pin;
    int direction;

    pin = inputStream.get();
    if (inputStream.good() != true)
    {
        return;
    }
    myPin = (unsigned int)pin;

    direction = inputStream.get();
    if (inputStream.good() != true)
    {
        return;
    }
    myDirection = ((direction == 1) ? DIR_INPUT : DIR_OUTPUT);

    inputStream.get();
}

bool
PinConfigInfoPacket::isValid() const
{
    return true;
}

bool
PinConfigInfoPacket::operator==(
        const Packet& packet
        ) const
{
    return true;
}

RedBotPacket::Type
PinConfigInfoPacket::getType() const
{
    return TYPE_PINCONFIGINFO;
}

unsigned int
PinConfigInfoPacket::getPin() const
{
    return myPin;
}

RedBotPacket::PinDirection
PinConfigInfoPacket::getDirection() const
{
    return myDirection;
}

