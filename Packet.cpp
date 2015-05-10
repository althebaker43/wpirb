
#include "Packet.h"
#include <sstream>


Packet*
Packet::Read(
        std::istream&   inputStream
        )
{
    if (inputStream.good() == false)
    {
        return NULL;
    }

    char header;
    inputStream.get(header);
    if (header == '\xFF')
    {
        Packet* packet = NULL;
        char packetType = '\0';
        inputStream.get(packetType);
        
        switch ((unsigned char)packetType)
        {
            case BID_PING:
                packet = new PingPacket();
                break;

            case BID_DOUTPUT:
                packet = new DigitalOutputPacket();
                break;

            case BID_DINPUT:
                packet = new DigitalInputPacket();
                break;

            case BID_DVALUE:
                packet = new DigitalValuePacket();
                break;

            case BID_ACK:
                packet = new AcknowledgePacket();
                break;

            default:
                break;
        };

        if (packet != NULL)
        {
            inputStream >> *packet;
            if (packet->isValid() == true)
            {
                return packet;
            }
            else
            {
                delete packet;
                return NULL;
            }
        }
    }

    return NULL;
}

bool
Packet::operator!=(
        const Packet&   packet
        ) const
{
    return !(*this == packet);
}

Packet::operator std::string() const
{
    std::ostringstream stringStream;
    write(stringStream);
    return stringStream.str();
}


std::ostream&
operator<<(
        std::ostream&   outputStream,
        const Packet&   packet
        )
{
    packet.write(outputStream);
    return outputStream;
}

std::istream&
operator>>(
        std::istream&   inputStream,
        Packet&         packet
        )
{
    packet.read(inputStream);
    return inputStream;
}


PingPacket::PingPacket() :
    myIsValid(true)
{
}

void
PingPacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream << '\xFF';
    outputStream << (unsigned char)BID_PING;
    outputStream << '\xFF';
}

void
PingPacket::read(
        std::istream& inputStream
        )
{
    unsigned char trailer;

    inputStream >> trailer;
    if (trailer == 0xFF)
    {
        myIsValid = true;
    }
    else
    {
        myIsValid = false;
    }
}

bool
PingPacket::isValid() const
{
    return myIsValid;
}

bool
PingPacket::operator==(
        const Packet&   packet
        ) const
{
    if (getType() == packet.getType())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Packet::Type
PingPacket::getType() const
{
    return TYPE_PING;
}


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
    if (getType() == packet.getType())
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

Packet::Type
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


DigitalInputPacket::DigitalInputPacket() :
    myPin(0)
{
}

DigitalInputPacket::DigitalInputPacket(
        unsigned int pin
        ) :
    myPin(pin)
{
}

void
DigitalInputPacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream << '\xFF';
    outputStream << (unsigned char)BID_DINPUT;
    outputStream << (unsigned char)myPin;
    outputStream << '\xFF';
}

void
DigitalInputPacket::read(
        std::istream& inputStream
        )
{
    int pin = -1;

    pin = inputStream.get();
    myPin = (unsigned int)pin;
    if (inputStream.good() == false)
    {
        return;
    }

    inputStream.get();
}

bool
DigitalInputPacket::isValid() const
{
    return true;
}

bool
DigitalInputPacket::operator==(
        const Packet& packet
        ) const
{
    return true;
}

Packet::Type
DigitalInputPacket::getType() const
{
    return TYPE_DINPUT;
}

unsigned int
DigitalInputPacket::getPin() const
{
    return myPin;
}


AcknowledgePacket::AcknowledgePacket() :
    myIsValid(true)
{
}

void
AcknowledgePacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream << '\xFF';
    outputStream << (unsigned char)BID_ACK;
    outputStream << '\xFF';
}

void
AcknowledgePacket::read(
        std::istream& inputStream
        )
{
    unsigned char trailer;

    inputStream >> trailer;
    if (trailer == 0xFF)
    {
        myIsValid = true;
    }
    else
    {
        myIsValid = false;
    }
}

bool
AcknowledgePacket::isValid() const
{
    return myIsValid;
}

bool
AcknowledgePacket::operator==(
        const Packet& packet
        ) const
{
    if (packet.getType() == TYPE_ACK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Packet::Type
AcknowledgePacket::getType() const
{
    return TYPE_ACK;
}


DigitalValuePacket::DigitalValuePacket() :
    myPin(0),
    myValue(false),
    myIsValid(false)
{
}

DigitalValuePacket::DigitalValuePacket(
        unsigned int    pin,
        bool            value
        ) :
    myPin(pin),
    myValue(value),
    myIsValid(true)
{
}

void
DigitalValuePacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream << '\xFF';
    outputStream << (unsigned char)BID_DVALUE;
    outputStream << (unsigned char)myPin;
    outputStream << ((myValue == true) ? '\x02' : '\x01');
    outputStream << '\xFF';
}

void
DigitalValuePacket::read(
        std::istream& inputStream
        )
{
    int pin = -1;
    int value = -1;
    int trailer;

    myIsValid = false;

    pin = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }
    myPin = (unsigned int)pin;

    value = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }
    myValue = (value == 0x02);

    // Read out trailer
    trailer = inputStream.get();
    if (trailer != 0xFF)
    {
        return;
    }

    myIsValid = true;
}

bool
DigitalValuePacket::isValid() const
{
    return true;
}

bool
DigitalValuePacket::operator==(
        const Packet& packet
        ) const
{
    return true;
}

Packet::Type
DigitalValuePacket::getType() const
{
    return TYPE_DVALUE;
}

unsigned int
DigitalValuePacket::getPin() const
{
    return myPin;
}

bool
DigitalValuePacket::getValue() const
{
    return myValue;
}
