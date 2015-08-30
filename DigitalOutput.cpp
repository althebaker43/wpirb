
#include "DigitalOutput.h"
#include "PinConfigPacket.h"
#include <stdlib.h>


DigitalOutputPacket::DigitalOutputPacket() :
    myPin(13),
    myValue(false)
{
}

DigitalOutputPacket::DigitalOutputPacket(
        unsigned int    pin,
        bool            value
        ) :
    myPin(pin),
    myValue(value)
{
}

void
DigitalOutputPacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream << '\xFF';
    outputStream << (unsigned char)BID_DOUTPUT;
    outputStream << (unsigned char)myPin;
    outputStream << (myValue ? '\x02' : '\x01');
    outputStream << '\xFF';
}

void
DigitalOutputPacket::writeXML(
        std::ostream& outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>DOUTPUT</type>"
        << "<pin>" << myPin << "</pin>"
        << "<value>" << ((myValue == true) ? "1" : "0") << "</value>"
        << "</packet>";
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

RedBotPacket::Type
DigitalOutputPacket::getType() const
{
    return TYPE_DOUTPUT;
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
    myChannel(channel),
    myCurrentPacket(NULL),
    myIsPinConfigured(false)
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

    if (myIsPinConfigured == true)
    {
        packet = myCurrentPacket;
        myCurrentPacket = NULL;
    }
    else
    {
        packet = new PinConfigPacket(
                myChannel,
                PinConfigPacket::DIR_OUTPUT
                );
        myIsPinConfigured = true;
    }

    return packet;
}

bool
DigitalOutput::processPacket(
        const Packet& packet
        )
{
    return false;
}

