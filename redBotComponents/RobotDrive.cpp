
#include "RobotDrive.h"
#include "RedBotSpeedController.h"
#include <stdlib.h>
#include <math.h>

using namespace frc;


DifferentialDrive::DifferentialDrive(
	SpeedController& leftController,
        SpeedController& rightController
        ) :
    RedBotComponent(),
    myLeftController(leftController),
    myRightController(rightController)
{
}

void
DifferentialDrive::SetExpiration(
        double expiration
        )
{
}

void
DifferentialDrive::ArcadeDrive(
        double magnitude,
        double curve,
        bool squaredInputs
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

    if (squaredInputs == true)
    {
      magnitude = ((magnitude < 0.0) ? -1.0 : 1.0) * pow(magnitude, 2.0);
      curve = ((curve < 0.0) ? -1.0 : 1.0) * pow(curve, 2.0);
    }

    myLeftController.Set(magnitude + curve);
    myRightController.Set(magnitude - curve);
}

Packet*
DifferentialDrive::getNextPacket()
{
  return NULL;
}

bool
DifferentialDrive::processPacket(
        const Packet& packet
        )
{
  return false;
}

SpeedController&
DifferentialDrive::getLeftController()
{
  return myLeftController;
}

SpeedController&
DifferentialDrive::getRightController()
{
  return myRightController;
}
