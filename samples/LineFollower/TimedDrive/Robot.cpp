
#include <WPILib.h>

class Robot : public frc::IterativeRobot
{
private:

  RedBotSpeedController myLeftMotor;
  RedBotSpeedController myRightMotor;
  enum DriveState { FORWARD, STOP_FORWARD, BACKWARD, STOP_BACKWARD };
  DriveState myState;
  frc::Timer myTimer;

public:

  Robot() :
    myLeftMotor(0),
    myRightMotor(1)
  {
  }

  void AutonomousInit()
  {
    myTimer.Stop();
    myTimer.Reset();
    myTimer.Start();

    myState = FORWARD;
    myLeftMotor.Set(0.6);
    myRightMotor.Set(0.6);
  }

  void AutonomousPeriodic()
  {
    if (myTimer.HasPeriodPassed(2.0) == false)
      {
	return;
      }

    double speed = 0.0;

    switch (myState)
      {
      case FORWARD:
	speed = 0.0;
	myState = STOP_FORWARD;
	break;

      case STOP_FORWARD:
	speed = -0.6;
	myState = BACKWARD;
	break;

      case BACKWARD:
	speed = 0.0;
	myState = STOP_BACKWARD;
	break;

      case STOP_BACKWARD:
	speed = 0.6;
	myState = FORWARD;
	break;
      };

    myTimer.Stop();
    myTimer.Reset();
    myTimer.Start();

    myLeftMotor.Set(speed);
    myRightMotor.Set(speed);
  }

  void DisabledInit()
  {
    myLeftMotor.Set(0.0);
    myRightMotor.Set(0.0);
  }
};

START_ROBOT_CLASS(Robot);
