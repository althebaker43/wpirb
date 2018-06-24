
#include <WPILib.h>

class Robot : public frc::IterativeRobot
{
private:

  RedBotSpeedController myLeftMotor;
  RedBotSpeedController myRightMotor;
  frc::AnalogInput myLeftSensor;
  frc::AnalogInput myMiddleSensor;
  frc::AnalogInput myRightSensor;

  const int LINE_THRESHOLD = 900;

public:

  Robot() :
    myLeftMotor(0),
    myRightMotor(1),
    myLeftSensor(3),
    myMiddleSensor(6),
    myRightSensor(7)
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

  bool isAtLine(int sensorValue)
  {
    if (sensorValue > LINE_THRESHOLD)
      {
	return true;
      }
    else
      {
	return false;
      }
  }

  void AutonomousPeriodic()
  {
    int leftValue = myLeftSensor.Get();
    int middleValue = myMiddleSensor.Get();
    int rightValue = myRightSensor.Get();

    if (isAtLine(middleValue) && !isAtLine(leftValue) && !isAtLine(rightValue))
      {
	myLeftMotor.Set(1.0);
	myRightMotor.Set(1.0);
      }
    else if (isAtLine(leftValue))
      {
	myLeftMotor.Set(-0.6);
	myRightMotor.Set(0.6);
      }
    else
      {
	myLeftMotor.Set(0.6);
	myRightMotor.Set(-0.6);
      }
  }
};

START_ROBOT_CLASS(Robot);
