#include "WPILib.h"
#include <stdio.h>

// Algorithm copied from SparkFun's DriveStraight example

class Robot : public frc::IterativeRobot
{
  RedBotSpeedController myLeftMotor;
  RedBotSpeedController myRightMotor;
  RedBotEncoder myLeftEncoder;
  RedBotEncoder myRightEncoder;
  int myPrevLeftTicks;
  int myPrevRightTicks;
  double myLeftThrust;
  double myRightThrust;
  size_t myPrintCounter;

  static const ssize_t NUM_FT = 3;
  static const ssize_t TICKS_PER_FT = 286;
  static constexpr double MAX_THRUST = 0.5;
  static constexpr double THRUST_OFFSET = 0.05;

public:

  Robot() :
    myLeftMotor(0),
    myRightMotor(1),
    myLeftEncoder(false),
    myRightEncoder(true),
    myPrevLeftTicks(0),
    myPrevRightTicks(0),
    myLeftThrust(MAX_THRUST),
    myRightThrust(MAX_THRUST),
    myPrintCounter(0)
  {
  }

private:

  void DisabledInit()
  {
    myLeftMotor.Set(0.0);
    myRightMotor.Set(0.0);
  }

  void DisabledPeriodic()
  {
    myLeftMotor.Set(0.0);
    myRightMotor.Set(0.0);
  }

  void AutonomousInit()
  {
    myLeftEncoder.Reset();
    myRightEncoder.Reset();
    myPrevLeftTicks = 0;
    myPrevRightTicks = 0;
    myPrintCounter = 0;

    std::cout << "Start" << std::endl;
  }

  void AutonomousPeriodic()
  {
    int leftTicks = myLeftEncoder.Get();
    int rightTicks = myRightEncoder.Get();

    if (leftTicks < (TICKS_PER_FT * NUM_FT))
      {
	myLeftMotor.Set(myLeftThrust);
	myRightMotor.Set(myRightThrust);

	int leftDiff = leftTicks - myPrevLeftTicks;
	int rightDiff = rightTicks - myPrevRightTicks;

	myPrevLeftTicks = leftTicks;
	myPrevRightTicks = rightTicks;

	const char* dir = "straight";

	if (leftDiff > rightDiff)
	  {
	    myLeftThrust = MAX_THRUST - THRUST_OFFSET;
	    myRightThrust = MAX_THRUST + THRUST_OFFSET;
	    dir = "left";
	  }
	else if (leftDiff < rightDiff)
	  {
	    myLeftThrust = MAX_THRUST + THRUST_OFFSET;
	    myRightThrust = MAX_THRUST - THRUST_OFFSET;
	    dir = "right";
	  }

	if (++myPrintCounter >= 1)
	  {
	    std::cout
	      << "Left: " << leftTicks << ", "
	      << "Right: " << rightTicks << ", "
	      << "LDiff: " << leftDiff << ", "
	      << "RDiff: " << rightDiff << ", "
	      << "Dir: " << dir << std::endl;
	    myPrintCounter = 0;
	  }
      }
    else
      {
	myLeftMotor.Set(0.0);
	myRightMotor.Set(0.0);
      }
  }
};

START_ROBOT_CLASS(Robot);
