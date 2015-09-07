
#include "DigitalOutput.h"
#include <stdlib.h>


DigitalOutputPacket::DigitalOutputPacket() :
    RedBotPacket(TYPE_DOUTPUT, "DOUTPUT", BID_DOUTPUT),
    myPin(13),
    myValue(false)
{
}

DigitalOutputPacket::DigitalOutputPacket(
        unsigned int    pin,
        bool            value
        ) :
    RedBotPacket(TYPE_DOUTPUT, "DOUTPUT", BID_DOUTPUT),
    myPin(pin),
    myValue(value)
{
}

void
DigitalOutputPacket::writeContents(
        std::ostream& outputStream
        ) const
{
    outputStream << (unsigned char)myPin;
    outputStream << (myValue ? '\x02' : '\x01');
}

void
DigitalOutputPacket::getXMLElements(
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
                "value",
                ((myValue == true) ? "1" : "0")
                )
            );
}

void
DigitalOutputPacket::read(
        std::istream& inputStream
        )
{
    unsigned char pin;
    unsigned char value;

    inputStream >> pin;
    myPin = (unsigned int)pin;
    if (inputStream.good() == false)
    {
        return;
    }

    inputStream >> value;
    if (value == '\x02')
    {
        myValue = true;
    }
    else
    {
        myValue = false;
    }
}

bool
DigitalOutputPacket::isValid() const
{
    return true;
}

bool
DigitalOutputPacket::operator==(
        const Packet& packet
        ) const
{
    if (NULL != dynamic_cast<const DigitalOutputPacket*>(&packet))
    {
        const DigitalOutputPacket& dOutPacket = static_cast<const DigitalOutputPacket&>(packet);

        if(
                (getPin() == dOutPacket.getPin()) &&
                (getValue() == dOutPacket.getValue())
          )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

unsigned int
DigitalOutputPacket::getPin() const
{
    return myPin;
}

bool
DigitalOutputPacket::getValue() const
{
    return myValue;
}


DigitalOutput::DigitalOutput(
        uint32_t channel
        ) :
    Component(),
    ConfigurableInterface(channel, RedBotPacket::DIR_OUTPUT),
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
    if (myCurrentPacket != NULL)
    {
        delete myCurrentPacket;
    }

    myCurrentPacket = new DigitalOutputPacket(
            myChannel,
            ((value == 0) ? false : true)
            );
}

Packet*
DigitalOutput::getNextPacket()
{
    Packet* packet = NULL;

    if (isConfigured() == true)
    {
        packet = myCurrentPacket;
        myCurrentPacket = NULL;
    }
    else
    {
        packet = getNextConfigPacket();
    }

    return packet;
}

bool
DigitalOutput::processPacket(
        const Packet& packet
        )
{
    if (isConfigured() == true)
    {
        return false;
    }
    else
    {
        return processConfigInfoPacket(packet);
    }
}

