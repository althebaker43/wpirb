
#include "Packet.h"
#include <sstream>
#include <vector>


Packet*
Packet::Read(
        std::istream&   inputStream,
        std::string*    readData
        )
{
    if (inputStream.good() == false)
    {
        return NULL;
    }

    if (readData != NULL)
    {
        readData->clear();
    }

    std::string tmpDataBuf;

    char header;
    inputStream.get(header);
    if (inputStream.good() == false)
    {
        return NULL;
    }
    tmpDataBuf.push_back(header);

    if (header == '\xFF')
    {
        Packet* packet = NULL;
        char packetType = '\0';
        inputStream.get(packetType);
        if (inputStream.good() == false)
        {
            return NULL;
        }
        tmpDataBuf.push_back(packetType);
        
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

            case BID_MDRIVE:
                packet = new MotorDrivePacket();
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

            if (readData != NULL)
            {
                // Dump read data for debugging
                std::ostringstream outputStream;
                outputStream << *packet;
                *readData = outputStream.str();
            }

            if (packet->isValid() == true)
            {
                return packet;
            }
            else
            {
                // Delete invalid packet
                delete packet;

                return NULL;
            }
        }
    }

    // Dump read data for debugging
    if (readData != NULL)
    {
        *readData = tmpDataBuf;
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
PingPacket::writeXML(
        std::ostream& outputStream
        ) const
{
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


MotorDrivePacket::MotorDrivePacket() :
    myMotor(MOTOR_RIGHT),
    mySpeed(0),
    myDirection(DIR_FORWARD)
{
}

MotorDrivePacket::MotorDrivePacket(
        MotorDrivePacket::Motor     motor,
        uint8_t                     speed,
        MotorDrivePacket::Direction direction
        ) :
    myMotor(motor),
    mySpeed(speed),
    myDirection(direction)
{
}

MotorDrivePacket::~MotorDrivePacket()
{
}

void
MotorDrivePacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream << '\xFF';
    outputStream << (unsigned char)BID_MDRIVE;
    outputStream << ((myMotor == MOTOR_LEFT) ? '\x02' : '\x01');
    outputStream << ((myDirection == DIR_FORWARD) ? '\x01' : '\x02');
    outputStream << (unsigned char)(((mySpeed & 0xF0) >> 4) + 1);
    outputStream << (unsigned char)((mySpeed & 0x0F) + 1);
    outputStream << '\xFF';
}

void
MotorDrivePacket::writeXML(
        std::ostream& outputStream
        ) const
{
    outputStream
        << "<packet>"
        << "<type>MDRIVE</type>"
        << "<motor>" << ((myMotor == MOTOR_RIGHT) ? "right" : "left") << "</motor>"
        << "<speed>" << (unsigned int)mySpeed << "</speed>"
        << "<direction>" << ((myDirection == DIR_FORWARD) ? "forward" : "backward") << "</direction>"
        << "</packet>";
}

void
MotorDrivePacket::read(
        std::istream& inputStream
        )
{
    std::vector<unsigned char> byteBuf(4);

    for (int i = 0; i < 4; i++)
    {
        unsigned char curByte;
        inputStream >> curByte;

        if(
                (curByte == '\xFF') ||
                (inputStream.good() == false)
          )
        {
            return;
        }

        byteBuf[i] = curByte;
    }

    myMotor = ((byteBuf[0] == '\x01') ? MOTOR_RIGHT : MOTOR_LEFT);
    mySpeed = ((byteBuf[2] - 1) << 4) | (byteBuf[3] - 1);
    myDirection = ((byteBuf[1] == '\x01') ? DIR_FORWARD : DIR_BACKWARD);

    unsigned char trailer;
    inputStream >> trailer;
}

bool
MotorDrivePacket::isValid() const
{
    return true;
}

bool
MotorDrivePacket::operator==(
        const Packet& packet
        ) const
{
    if (packet.getType() != TYPE_MDRIVE)
    {
        return false;
    }

    const MotorDrivePacket& mDrivePacket = static_cast<const MotorDrivePacket&>(packet);

    if(
            (mDrivePacket.getMotor() == myMotor) &&
            (mDrivePacket.getSpeed() == mySpeed) &&
            (mDrivePacket.getDirection() == myDirection)
      )
    {
        return true;
    }
    else
    {
        return false;
    }
}

Packet::Type
MotorDrivePacket::getType() const
{
    return TYPE_MDRIVE;
}

MotorDrivePacket::Motor
MotorDrivePacket::getMotor() const
{
    return myMotor;
}

uint8_t
MotorDrivePacket::getSpeed() const
{
    return mySpeed;
}

MotorDrivePacket::Direction
MotorDrivePacket::getDirection() const
{
    return myDirection;
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
AcknowledgePacket::writeXML(
        std::ostream& outputStream
        ) const
{
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

    if (isValid() == true)
    {
        outputStream << (unsigned char)myPin;
        outputStream << ((myValue == true) ? '\x02' : '\x01');
        outputStream << '\xFF';
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

    pin = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }
    myBinaryData.push_back(pin);
    myPin = (unsigned int)pin;

    value = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }
    myBinaryData.push_back(value);
    myValue = (value == 0x02);

    // Read out trailer
    trailer = inputStream.get();
    if (inputStream.good() == false)
    {
        return;
    }
    myBinaryData.push_back(trailer);
    if (trailer != 0xFF)
    {
        return;
    }

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
