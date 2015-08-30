
#include "RobotDrive.h"
#include "Joystick.h"
#include <stdlib.h>


MotorDrivePacket::MotorDrivePacket() :
    RedBotPacket(BID_MDRIVE),
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
    RedBotPacket(BID_MDRIVE),
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
    RedBotPacket(BID_MDRIVE),
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
MotorDrivePacket::writeContents(
        std::ostream& outputStream
        ) const
{
    outputStream << ((myMotor == MOTOR_LEFT) ? '\x02' : '\x01');
    outputStream << ((myDirection == DIR_FORWARD) ? '\x01' : '\x02');
    outputStream << (unsigned char)(((mySpeed & 0xF0) >> 4) + 1);
    outputStream << (unsigned char)((mySpeed & 0x0F) + 1);
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
    if (NULL == dynamic_cast<const MotorDrivePacket*>(&packet))
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

RedBotPacket::Type
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


RobotDrive::RobotDrive(
        uint32_t leftChannel,
        uint32_t rightChannel
        ) :
    Component(),
    myCurrentLMotorPacket(NULL),
    myCurrentRMotorPacket(NULL)
{
}

void
RobotDrive::SetExpiration(
        double expiration
        )
{
}

void
RobotDrive::ArcadeDrive(
        Joystick& joystick
        )
{
    Drive(
            joystick.GetY(),
            joystick.GetX()
         );
}

void
RobotDrive::Drive(
        double magnitude,
        double curve
        )
{
    // Check bounds on magnitude
    if(
            (magnitude < -1.0) ||
            (magnitude > 1.0)
      )
    {
        return;
    }

    if (myCurrentLMotorPacket != NULL)
    {
        delete myCurrentLMotorPacket;
    }

    myCurrentLMotorPacket = new MotorDrivePacket(
            MotorDrivePacket::MOTOR_LEFT,
            (magnitude + curve)
            );

    if (myCurrentRMotorPacket != NULL)
    {
        delete myCurrentRMotorPacket;
    }

    myCurrentRMotorPacket = new MotorDrivePacket(
            MotorDrivePacket::MOTOR_RIGHT,
            (magnitude - curve)
            );
}

Packet*
RobotDrive::getNextPacket()
{
    if (myCurrentRMotorPacket != NULL)
    {
        Packet* rMotorPacket = myCurrentRMotorPacket;
        myCurrentRMotorPacket = NULL;
        return rMotorPacket;
    }
    else if (myCurrentLMotorPacket != NULL)
    {
        Packet* lMotorPacket = myCurrentLMotorPacket;
        myCurrentLMotorPacket = NULL;
        return lMotorPacket;
    }
    else
    {
        return NULL;
    }
}

bool
RobotDrive::processPacket(
        const Packet& packet
        )
{
    return false;
}
