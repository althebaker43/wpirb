
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
  frc::Timer myRushTimer;
  bool myRushActive;
  bool myIsTurningLeft;
  bool myIsTurningRight;
  double myTurnTime;

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
    myTurnInwardPower(0.4),
    myRushActive(false),
    myIsTurningLeft(false),
    myIsTurningRight(false),
    myTurnTime(1.2)
  {
    frc::SmartDashboard::init();
    if (!frc::SmartDashboard::PutNumber("Actual Drive Forward Power", myDriveForwardPower))
      {
	std::cout << "Warning: could not initialize SmartDashboard." << std::endl;
      }
	
    if (!frc::SmartDashboard::PutNumber("Actual Turn Outward Power", myTurnOutwardPower))
      {
	std::cout << "Warning: could not initialize SmartDashboard." << std::endl;
      }

    if (!frc::SmartDashboard::PutNumber("Actual Turn Inward Power", myTurnInwardPower))
      {
	std::cout << "Warning: could not initialize SmartDashboard." << std::endl;
      }
  }

  void DisabledInit()
  {
    myLeftMotor.Set(0.0);
    myRightMotor.Set(0.0);
  }

  void AutonomousInit()
  {
    myDriveForwardPower = frc::SmartDashboard::GetNumber("Set Drive Forward Power", myDriveForwardPower);
    myTurnOutwardPower = frc::SmartDashboard::GetNumber("Set Turn Outward Power", myTurnOutwardPower);
    myTurnInwardPower = frc::SmartDashboard::GetNumber("Set Turn Inward Power", myTurnInwardPower);
    myTurnTime = frc::SmartDashboard::GetNumber("Set Turn Time", myTurnTime);

    if (!frc::SmartDashboard::PutNumber("Actual Drive Forward Power", myDriveForwardPower))
      {
	std::cout << "Warning: could not initialize SmartDashboard." << std::endl;
      }
	
    if (!frc::SmartDashboard::PutNumber("Actual Turn Outward Power", myTurnOutwardPower))
      {
	std::cout << "Warning: could not initialize SmartDashboard." << std::endl;
      }

    if (!frc::SmartDashboard::PutNumber("Actual Turn Inward Power", myTurnInwardPower))
      {
	std::cout << "Warning: could not initialize SmartDashboard." << std::endl;
      }

    std::cout << "Drive Forward Power: " << myDriveForwardPower << std::endl
	      << "Turn Outward Power: " << myTurnOutwardPower << std::endl
	      << "Turn Inward Power: " << myTurnInwardPower << std::endl
	      << "Turn Time: " << myTurnTime << std::endl;

    myRushActive = false;
    myIsTurningLeft = false;
    myIsTurningRight = false;
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
    if (myRushActive && !myRushTimer.HasPeriodPassed(0.0))
      {
	return;
      }
    else
      {
	myRushActive = false;

	if (myIsTurningLeft && !myRushTimer.HasPeriodPassed(myTurnTime))
	  {
	    myLeftMotor.Set(myTurnInwardPower);
	    myRightMotor.Set(myTurnOutwardPower);
	    return;
	  }
	if (myIsTurningRight && !myRushTimer.HasPeriodPassed(myTurnTime))
	  {
	    myLeftMotor.Set(myTurnOutwardPower);
	    myRightMotor.Set(myTurnInwardPower);
	    return;
	  }
      }

    myIsTurningLeft = false;
    myIsTurningRight = false;

    int leftValue = myLeftSensor.Get();
    int middleValue = myMiddleSensor.Get();
    int rightValue = myRightSensor.Get();

    if (isAtLine(middleValue) && !isAtLine(leftValue) && !isAtLine(rightValue))
      {
	myLeftMotor.Set(myDriveForwardPower);
	myRightMotor.Set(myDriveForwardPower);
      }
    else if (isAtLine(middleValue) && (isAtLine(leftValue)))
      {
	myLeftMotor.Set(myDriveForwardPower);
	myRightMotor.Set(myDriveForwardPower);
	myRushTimer.Reset();
	myRushTimer.Start();
	myRushActive = true;
	myIsTurningLeft = true;
	std::cout << "Rushing..." << std::endl;
      }
    else if (isAtLine(middleValue) && (isAtLine(rightValue)))
      {
	myLeftMotor.Set(myDriveForwardPower);
	myRightMotor.Set(myDriveForwardPower);
	myRushTimer.Reset();
	myRushTimer.Start();
	myRushActive = true;
	myIsTurningRight = true;
	std::cout << "Rushing..." << std::endl;
      }
    else if (isAtLine(leftValue))
      {
	myLeftMotor.Set(myDriveForwardPower*0.5);
	myRightMotor.Set(myDriveForwardPower);
      }
    else
      {
	myLeftMotor.Set(myDriveForwardPower);
	myRightMotor.Set(myDriveForwardPower*0.5);
      }
  }
};

START_ROBOT_CLASS(Robot);
