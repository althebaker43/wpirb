#include "WPILib.h"

class Robot: public frc::IterativeRobot
{
	RedBotSpeedController lMotor;
	RedBotSpeedController rMotor;
	frc::DifferentialDrive myRobot; // robot drive system
        frc::Joystick stick; // only joystick
	LiveWindow *lw;
	int autoLoopCounter;

public:
	Robot() :
		lMotor(0),
		rMotor(1),
		myRobot(lMotor, rMotor),	// these must be initialized in the same order
		stick(0),		// as they are declared above.
		lw(NULL),
		autoLoopCounter(0)
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic()
	{
		if(autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
		{
			myRobot.ArcadeDrive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
			} else {
			myRobot.ArcadeDrive(0.0, 0.0); 	// stop robot
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(stick.GetY(), stick.GetX()); // drive with arcade style (use right stick)
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
