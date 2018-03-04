#include "WPILib.h"
#include "SmartDashboard.h"
#include <llvm/StringRef.h>
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

	  frc::SmartDashboard::init();
	  frc::SmartDashboard::PutBoolean("Found Cube", false);
	  frc::SmartDashboard::PutNumber("Cube X", 0);
	  frc::SmartDashboard::PutNumber("Cube Y", 0);
	  frc::SmartDashboard::PutNumber("Drive X", 0.0);
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
		  frc::SmartDashboard::PutBoolean("Found Cube", false);
		  return;
		}

	      float xValue = ((float)(block.x - 160))/160;

	      frc::SmartDashboard::PutBoolean("Found Cube", true);
	      frc::SmartDashboard::PutNumber("Cube X", block.x);
	      frc::SmartDashboard::PutNumber("Cube Y", block.y);
	      frc::SmartDashboard::PutNumber("Drive X", xValue);

	      drive.ArcadeDrive(joystick.GetY(), xValue);
	    }
        }
};

START_ROBOT_CLASS(Robot);
