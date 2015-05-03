
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
        
        switch (packetType)
        {
            case '\x01':
                packet = new PingPacket();
                break;

            case '\x02':
                packet = new DigitalOutputPacket();
                break;

            case '\x81':
                packet = new DigitalValuePacket();
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
    outputStream << "\xFF\x01\xFF";
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
    outputStream << "\xFF\x02";
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
    outputStream << "\xFF\x03";
    outputStream << (unsigned char)myPin;
    outputStream << '\xFF';
}

void
DigitalInputPacket::read(
        std::istream& inputStream
        )
{
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


DigitalValuePacket::DigitalValuePacket() :
    myPin(0),
    myValue(false)
{
}

DigitalValuePacket::DigitalValuePacket(
        unsigned int    pin,
        bool            value
        ) :
    myPin(pin),
    myValue(value)
{
}

void
DigitalValuePacket::write(
        std::ostream& outputStream
        ) const
{
}

void
DigitalValuePacket::read(
        std::istream& inputStream
        )
{
    int pin = -1;
    int value = -1;

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
    inputStream.get();
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
