
#include <WPILib.h>
#include <llvm/StringRef.h>

class Robot : public frc::IterativeRobot
{
private:

  frc::AnalogInput myLeftSensor;
  frc::AnalogInput myMiddleSensor;
  frc::AnalogInput myRightSensor;

  const int LINE_THRESHOLD = 900;

public:

  Robot() :
    myLeftSensor(3),
    myMiddleSensor(6),
    myRightSensor(7)
  {
    frc::SmartDashboard::init();
  }

  void DisabledInit()
  {
  }

  void AutonomousInit()
  {
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

    frc::SmartDashboard::PutNumber("Left Sensor", leftValue);
    frc::SmartDashboard::PutNumber("Middle Sensor", middleValue);
    frc::SmartDashboard::PutNumber("Right Sensor", rightValue);

    frc::SmartDashboard::PutBoolean("Left at Line", isAtLine(leftValue));
    frc::SmartDashboard::PutBoolean("Middle at Line", isAtLine(middleValue));
    frc::SmartDashboard::PutBoolean("Right at Line", isAtLine(rightValue));
  }
};

START_ROBOT_CLASS(Robot);
