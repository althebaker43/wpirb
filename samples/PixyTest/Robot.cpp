#include "WPILib.h"
#include <pixy.h>

class Robot : public frc::IterativeRobot
{
    private:

        RedBotSpeedController lMotor;
        RedBotSpeedController rMotor;
        frc::DifferentialDrive drive;
        frc::Joystick joystick;

  static bool CheckPixyStatus(int status)
  {
    if (status >= 0)
      {
	return true;
      }

    std::cout << "Error: PixyCam error: ";
    switch (status)
      {
      case PIXY_ERROR_USB_IO:
	std::cout << "I/O";
	break;

      case PIXY_ERROR_USB_NOT_FOUND:
	std::cout << "Pixy not found";
	break;

      case PIXY_ERROR_USB_BUSY:
	std::cout << "Busy";
	break;

      case PIXY_ERROR_USB_NO_DEVICE:
	std::cout << "No device";
	break;

      case PIXY_ERROR_INVALID_PARAMETER:
	std::cout << "Invalid parameter";
	break;

      default:
	std::cout << "Unknown";
	break;
      };
    std::cout << std::endl;

    return false;
  }

    public:

        Robot() :
            IterativeRobot(),
	    lMotor(0),
	    rMotor(1),
            drive(lMotor, rMotor),
            joystick(0)
        {
	  CheckPixyStatus(pixy_init());
	}

        void
        DisabledInit()
        {
            drive.ArcadeDrive(0.0, 0.0);
        }

        void
        TeleopPeriodic()
        {
	  if (!joystick.GetRawButton(0))
	    {
	      drive.ArcadeDrive(joystick.GetY(), joystick.GetX());
	    }
	  else
	    {
	      struct Block block;
	      int numBlocks = pixy_get_blocks(1, &block);
	      if ((numBlocks == 0) || (!CheckPixyStatus(numBlocks)))
		{
		  return;
		}

	      std::cout << "Found block at (" << block.x << ", " << block.y << ")" << std::endl;
	    }
        }
};

START_ROBOT_CLASS(Robot);
