#include "WPILib.h"
#include <stdio.h>

class Robot : public frc::IterativeRobot
{
    RedBotSpeedController myLeftMotor;
    RedBotSpeedController myRightMotor;
    frc::DifferentialDrive myDrive;
    frc::Timer myDriveTimer;
    bool myIsDrivingForward;

    public:

        Robot() :
	    myLeftMotor(0),
	    myRightMotor(1),
	    myDrive(myLeftMotor, myRightMotor),
            myDriveTimer(),
            myIsDrivingForward(true)
        {
        }

    private:

        void RobotInit()
        {
        }

        void DisabledInit()
        {
            myDriveTimer.Start();
            myDrive.ArcadeDrive(1.0, 0.0);
        }

        void DisabledPeriodic()
        {
            if (myDriveTimer.HasPeriodPassed(5.0) == true)
            {
                myDriveTimer.Reset();
                myIsDrivingForward = !myIsDrivingForward;
                myDrive.ArcadeDrive((myIsDrivingForward ? 1.0 : -1.0), 0.0);
                printf("Program: driving %s\n", (myIsDrivingForward ? "forward" : "backward"));
            }
        }
};

START_ROBOT_CLASS(Robot);
