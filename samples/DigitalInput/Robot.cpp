
#include "WPILib.h"
#include <iostream>

class Robot : public frc::IterativeRobot
{
    private:

        frc::DigitalInput dIn;
        bool lastVal;

    public:

        Robot() :
            IterativeRobot(),
            dIn(3),
            lastVal(false)
        {
	  frc::SmartDashboard::init();
        }

        void DisabledInit()
        {
            lastVal = dIn.Get();
            std::cout << "Program: Info: input initialized to " << lastVal << std::endl;
	    frc::SmartDashboard::PutBoolean("Input Value", lastVal);
        }

        void DisabledPeriodic()
        {
            bool curVal = dIn.Get();
            if (curVal != lastVal)
            {
                std::cout << "Program: Info: input changed to " << curVal << std::endl;
                lastVal = curVal;
		frc::SmartDashboard::PutBoolean("Input Value", lastVal);
            }
        }
};

START_ROBOT_CLASS(Robot);
