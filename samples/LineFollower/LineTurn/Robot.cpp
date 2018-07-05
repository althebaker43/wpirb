
#include <WPILib.h>
#include <llvm/StringRef.h>

class Robot : public frc::IterativeRobot
{
private:

  RedBotSpeedController myLeftMotor;
  RedBotSpeedController myRightMotor;
  frc::AnalogInput myLeftSensor;
  frc::AnalogInput myMiddleSensor;
  frc::AnalogInput myRightSensor;
  double myDriveForwardPower;
  double myTurnOutwardPower;
  double myTurnInwardPower;

  const int LINE_THRESHOLD = 900;

public:

  Robot() :
    myLeftMotor(0),
    myRightMotor(1),
    myLeftSensor(3),
    myMiddleSensor(6),
    myRightSensor(7),
    myDriveForwardPower(0.6),
    myTurnOutwardPower(0.6),
    myTurnInwardPower(0.4)
  {
    frc::SmartDashboard::init();
    frc::SmartDashboard::PutNumber("Drive Forward Power", myDriveForwardPower);
    frc::SmartDashboard::PutNumber("Turn Outward Power", myTurnOutwardPower);
    frc::SmartDashboard::PutNumber("Turn Inward Power", myTurnInwardPower);
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

    myDriveForwardPower = frc::SmartDashboard::GetNumber("Drive Forward Power", myDriveForwardPower);
    myTurnOutwardPower = frc::SmartDashboard::GetNumber("Turn Outward Power", myTurnOutwardPower);
    myTurnInwardPower = frc::SmartDashboard::GetNumber("Turn Inward Power", myTurnInwardPower);
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
	myLeftMotor.Set(myDriveForwardPower);
	myRightMotor.Set(myDriveForwardPower);
      }
    else if (isAtLine(leftValue))
      {
	myLeftMotor.Set(myTurnInwardPower);
	myRightMotor.Set(myTurnOutwardPower);
      }
    else
      {
	myLeftMotor.Set(myTurnOutwardPower);
	myRightMotor.Set(myTurnInwardPower);
      }
  }
};

START_ROBOT_CLASS(Robot);
