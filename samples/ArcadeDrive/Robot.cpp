#include "WPILib.h"

class Robot : public frc::IterativeRobot
{
    private:

        RedBotSpeedController lMotor;
        RedBotSpeedController rMotor;
        frc::DifferentialDrive drive;
        frc::Joystick joystick;

    public:

        Robot() :
            IterativeRobot(),
	    lMotor(0),
	    rMotor(1),
            drive(lMotor, rMotor),
            joystick(0)
        {
        }

        void
        DisabledInit()
        {
            drive.ArcadeDrive(0.0, 0.0);
        }

        void
        TeleopPeriodic()
        {
	    drive.ArcadeDrive(joystick.GetY(), joystick.GetX());
        }
};

START_ROBOT_CLASS(Robot);
