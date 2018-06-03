
#include "RedBotSpeedController.h"
#include <math.h>


MotorDrivePacket::MotorDrivePacket() :
    RedBotPacket(TYPE_MDRIVE, "MDRIVE", BID_MDRIVE),
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
    RedBotPacket(TYPE_MDRIVE, "MDRIVE", BID_MDRIVE),
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
    RedBotPacket(TYPE_MDRIVE, "MDRIVE", BID_MDRIVE),
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
MotorDrivePacket::getXMLElements(
        XMLElements& elements
        ) const
{
    elements.add(
            new XMLDataElement<std::string>(
                "motor",
                ((myMotor == MOTOR_RIGHT) ? "right" : "left")
                )
            );
    elements.add(
            new XMLDataElement<unsigned int>(
                "speed",
                (unsigned int)mySpeed
                )
            );
    elements.add(
            new XMLDataElement<std::string>(
                "direction",
                ((myDirection == DIR_FORWARD) ? "forward" : "backward")
                )
            );
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


RedBotSpeedController::RedBotSpeedController(size_t channel) :
  myChannel(channel),
  myCurrentMotorPacket(NULL)
{
}

void
RedBotSpeedController::Set(double speed)
{
  if (myCurrentMotorPacket != NULL)
    {
      delete myCurrentMotorPacket;
    }

  myCurrentMotorPacket = new MotorDrivePacket(((myChannel == 0) ? MotorDrivePacket::MOTOR_LEFT : MotorDrivePacket::MOTOR_RIGHT), speed);
}

Packet*
RedBotSpeedController::getNextPacket()
{
  if (myCurrentMotorPacket != NULL)
    {
      Packet* motorPacket = myCurrentMotorPacket;
      myCurrentMotorPacket = NULL;
      return motorPacket;
    }
  else
    {
      return NULL;
    }
}

bool
RedBotSpeedController::processPacket(const Packet& packet)
{
  return false;
}
