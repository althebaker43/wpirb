
#include "DigitalInput.h"
#include "PinConfigPacket.h"
#include <stdlib.h>


DigitalInputPacket::DigitalInputPacket() :
    RedBotPacket(BID_DINPUT),
    myPin(0)
{
}

DigitalInputPacket::DigitalInputPacket(
        unsigned int pin
        ) :
    RedBotPacket(BID_DINPUT),
    myPin(pin)
{
}

void
DigitalInputPacket::writeContents(
        std::ostream& outputStream
        ) const
{
    outputStream << (unsigned char)myPin;
}

void
DigitalInputPacket::writeXML(
        std::ostream& outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>DINPUT</type>"
        << "<pin>" << myPin << "</pin>"
        << "</packet>";
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

RedBotPacket::Type
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
    RedBotPacket(BID_DVALUE),
    myPin(0),
    myValue(false),
    myIsValid(false)
{
}

DigitalValuePacket::DigitalValuePacket(
        unsigned int    pin,
        bool            value
        ) :
    RedBotPacket(BID_DVALUE),
    myPin(pin),
    myValue(value),
    myIsValid(true)
{
}

void
DigitalValuePacket::writeContents(
        std::ostream& outputStream
        ) const
{
    if (isValid() == true)
    {
        outputStream << (unsigned char)myPin;
        outputStream << ((myValue == true) ? '\x02' : '\x01');
    }
    else
    {
        // Dump raw data read from input stream
        for(
                size_t byteIdx = 0;
                byteIdx < myBinaryData.size();
                ++byteIdx
           )
        {
            outputStream << (unsigned char)myBinaryData[byteIdx];
        }
    }
}

void
DigitalValuePacket::writeXML(
        std::ostream& outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>DVALUE</type>"
        << "<pin>" << myPin << "</pin>"
        << "<value>" << ((myValue == true) ? "1" : "0") << "</value>"
        << "</packet>";
}

void
DigitalValuePacket::read(
        std::istream& inputStream
        )
{
    int pin = -1;
    int value = -1;
    int trailer;

    myBinaryData.clear();
    myIsValid = false;

    for(
            size_t byteIdx = 2;
            byteIdx <= 4;
            ++byteIdx
       )
    {
        int curByte = inputStream.get();
        if (inputStream.good() == false)
        {
            break;
        }

        if (curByte == BINARY_BOUND)
        {
            trailer = curByte;
            break;
        }

        myBinaryData.push_back(curByte);

        switch (byteIdx)
        {
            case 2: pin = curByte; break;
            case 3: value = curByte; break;
            default: break;
        };
    }

    if(
            (pin == -1) ||
            (value == -1) ||
            (trailer != BINARY_BOUND)
      )
    {
        return;
    }

    myPin = (unsigned int)pin;
    myValue = (value == 0x02);

    myBinaryData.clear();
    myIsValid = true;
}

bool
DigitalValuePacket::isValid() const
{
    return myIsValid;
}

bool
DigitalValuePacket::operator==(
        const Packet& packet
        ) const
{
    return true;
}

RedBotPacket::Type
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


DigitalInput::DigitalInput(
        uint32_t channel
        ) :
    Input(channel),
    myIsPinConfigured(false)
{
}

DigitalInput::~DigitalInput()
{
}

Packet*
DigitalInput::getNextPacket()
{
    Packet* packet = NULL;

    if (myIsPinConfigured == true)
    {
        packet = getNextPacketIfTimedOut();
    }
    else
    {
        packet = new PinConfigPacket(
                myChannel,
                PinConfigPacket::DIR_INPUT
                );
        myIsPinConfigured = true;
    }

    return packet;
}

