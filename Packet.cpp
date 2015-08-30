
#include "Packet.h"
#include <sstream>
#include <vector>
#include <math.h>


Packet*
Packet::Read(
        std::istream&       inputStream,
        PacketGenerator&    packetGen,
        std::string*        readData
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

        packet = packetGen.createPacket((unsigned char)packetType);

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


Packet*
RedBotPacketGenerator::createPacket(
        unsigned char type
        )
{
    switch (type)
    {
        case Packet::BID_PING:          return new PingPacket(); break;
        case Packet::BID_PINCONFIG:     return new PinConfigPacket(); break;
        case Packet::BID_PINCONFIGINFO: return new PinConfigInfoPacket(); break;
        case Packet::BID_DINPUT:        return new DigitalInputPacket(); break;
        case Packet::BID_DOUTPUT:       return new DigitalOutputPacket(); break;
        case Packet::BID_DVALUE:        return new DigitalValuePacket(); break;
        case Packet::BID_AINPUT:        return new AnalogInputPacket(); break;
        case Packet::BID_AVALUE:        return new AnalogValuePacket(); break;
        case Packet::BID_MDRIVE:        return new MotorDrivePacket(); break;
        case Packet::BID_ACK:           return new AcknowledgePacket(); break;
        default: return NULL; break;
    };

    return NULL;
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

Packet::Type
AnalogInputPacket::getType() const
{
    return TYPE_AINPUT;
}

unsigned int
AnalogInputPacket::getPin() const
{
    return myPin;
}


PinConfigPacket::PinConfigPacket() :
    myPin(1),
    myDirection(DIR_INPUT)
{
}

PinConfigPacket::PinConfigPacket(
        unsigned int                pin,
        Packet::PinDirection        dir
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

Packet::Type
PinConfigPacket::getType() const
{
    return TYPE_PINCONFIG;
}

unsigned int
PinConfigPacket::getPin() const
{
    return myPin;
}

Packet::PinDirection
PinConfigPacket::getDirection() const
{
    return myDirection;
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

MotorDrivePacket::MotorDrivePacket(
        MotorDrivePacket::Motor motor,
        double                  driveVal
        ) :
    myMotor(motor)
{
    if (driveVal > 1.0)
    {
        driveVal = 1.0;
    }
    else if (driveVal < -1.0)
    {
        driveVal = -1.0;
    }

    mySpeed = fabs(driveVal) * 255;

    if (driveVal < 0.0)
    {
        myDirection = MotorDrivePacket::DIR_BACKWARD;
    }
    else
    {
        myDirection = MotorDrivePacket::DIR_FORWARD;
    }
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

Packet::Type
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


PinConfigInfoPacket::PinConfigInfoPacket() :
    myPin(1),
    myDirection(DIR_INPUT)
{
}

PinConfigInfoPacket::PinConfigInfoPacket(
        unsigned int            pin,
        Packet::PinDirection    dir
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

Packet::Type
PinConfigInfoPacket::getType() const
{
    return TYPE_PINCONFIGINFO;
}

unsigned int
PinConfigInfoPacket::getPin() const
{
    return myPin;
}

Packet::PinDirection
PinConfigInfoPacket::getDirection() const
{
    return myDirection;
}
