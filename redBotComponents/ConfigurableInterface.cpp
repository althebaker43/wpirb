
#include "ConfigurableInterface.h"


PinConfigPacket::PinConfigPacket() :
    RedBotPacket(TYPE_PINCONFIG, "PINCONFIG", BID_PINCONFIG),
    myPin(1),
    myDirection(DIR_INPUT)
{
}

PinConfigPacket::PinConfigPacket(
        unsigned int                pin,
        RedBotPacket::PinDirection  dir
        ) :
    RedBotPacket(TYPE_PINCONFIG, "PINCONFIG", BID_PINCONFIG),
    myPin(pin),
    myDirection(dir)
{
}

void
PinConfigPacket::writeContents(
        std::ostream& outputStream
        ) const
{
    outputStream
        << (unsigned char)myPin
        << (unsigned char)myDirection;
}

void
PinConfigPacket::getXMLElements(
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
            new XMLDataElement<std::string>(
                "direction",
                ((myDirection == DIR_OUTPUT) ? "OUTPUT" : "INPUT")
                )
            );
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
    RedBotPacket(TYPE_PINCONFIGINFO, "PINCONFIGINFO", BID_PINCONFIGINFO),
    myPin(1),
    myDirection(DIR_INPUT)
{
}

PinConfigInfoPacket::PinConfigInfoPacket(
        unsigned int                pin,
        RedBotPacket::PinDirection  dir
        ) :
    RedBotPacket(TYPE_PINCONFIGINFO, "PINCONFIGINFO", BID_PINCONFIGINFO),
    myPin(pin),
    myDirection(dir)
{
}

void
PinConfigInfoPacket::writeContents(
        std::ostream& outputStream
        ) const
{
    outputStream
        << (unsigned char)myPin
        << (unsigned char)myDirection;
}

void
PinConfigInfoPacket::getXMLElements(
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
            new XMLDataElement<std::string>(
                "direction",
                ((myDirection == DIR_INPUT) ? "INPUT" : "OUTPUT")
                )
            );
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
    myDirection = ((direction == 1) ? DIR_OUTPUT : DIR_INPUT);

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


ConfigurableInterface::ConfigurableInterface(
        unsigned int                pin,
        RedBotPacket::PinDirection  dir
        ) :
    myPin(pin),
    myDirection(dir),
    myIsPinConfigured(false),
    myConfigPacket(NULL)
{
}

Packet*
ConfigurableInterface::getNextConfigPacket()
{
    Packet* packet = NULL;

    // Only send one configuration packet per cycle
    if (myConfigPacket == NULL)
    {
        myConfigPacket = new PinConfigPacket(
                myPin,
                myDirection
                );
        packet = myConfigPacket;
    }
    else
    {
        myConfigPacket = NULL;
    }

    return packet;
}

bool
ConfigurableInterface::processConfigInfoPacket(
        const Packet& packet
        )
{
    const PinConfigInfoPacket* configInfoPacket = dynamic_cast<const PinConfigInfoPacket*>(&packet);
    if (configInfoPacket == NULL)
    {
        return false;
    }

    if (configInfoPacket->getPin() != myPin)
    {
        return false;
    }

    if (configInfoPacket->getDirection() == myDirection)
    {
        myIsPinConfigured = true;
    }

    return true;
}

bool
ConfigurableInterface::isConfigured() const
{
    return myIsPinConfigured;
}

