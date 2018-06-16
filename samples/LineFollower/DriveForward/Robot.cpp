
#include <WPILib.h>

class Robot : public frc::IterativeRobot
{
private:

  RedBotSpeedController myLeftMotor;
  RedBotSpeedController myRightMotor;

public:

  Robot() :
    myLeftMotor(0),
    myRightMotor(1)
  {
  }

  void DisabledInit()
  {
    myLeftMotor.Set(0.0);
    myRightMotor.Set(0.0);
  }

  void AutonomousInit()
  {
    myLeftMotor.Set(1.0);
    myRightMotor.Set(1.0);
  }

  void AutonomousPeriodic()
  {
  }
};

START_ROBOT_CLASS(Robot);
