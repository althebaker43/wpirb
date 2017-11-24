#include "WPILib.h"

class Robot : public frc::IterativeRobot
{
    private:

        RobotDrive drive;
        frc::Joystick joystick;

    public:

        Robot() :
            IterativeRobot(),
            drive(0, 0),
            joystick(0)
        {
        }

        void
        DisabledInit()
        {
            drive.Drive(0.0, 0.0);
        }

        void
        TeleopPeriodic()
        {
            drive.ArcadeDrive(joystick);
        }
};

START_ROBOT_CLASS(Robot);
