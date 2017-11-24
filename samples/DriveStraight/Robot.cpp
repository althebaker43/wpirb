#include "WPILib.h"
#include <stdio.h>

class Robot : public frc::IterativeRobot
{
    RobotDrive myDrive;
    Timer myDriveTimer;
    bool myIsDrivingForward;

    public:

        Robot() :
            myDrive(0, 0),
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
            myDrive.Drive(1.0, 0.0);
        }

        void DisabledPeriodic()
        {
            if (myDriveTimer.HasPeriodPassed(5.0) == true)
            {
                myDriveTimer.Reset();
                myIsDrivingForward = !myIsDrivingForward;
                myDrive.Drive((myIsDrivingForward ? 1.0 : -1.0), 0.0);
                printf("Program: driving %s\n", (myIsDrivingForward ? "forward" : "backward"));
            }
        }
};

START_ROBOT_CLASS(Robot);
