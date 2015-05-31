
#include "RobotDrive.h"
#include "Joystick.h"
#include "Packet.h"
#include <stdlib.h>


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
