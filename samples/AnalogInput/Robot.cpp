
#include "WPILib.h"
#include <iostream>

class Robot : public IterativeRobot
{
    private:

        AnalogInput aIn;
        Timer mySampleTimer;

    public:

        Robot() :
            IterativeRobot(),
            aIn(1),
            mySampleTimer()
        {
        }

        void DisabledInit()
        {
            mySampleTimer.Start();
        }

        void DisabledPeriodic()
        {
            if (mySampleTimer.HasPeriodPassed(0.5) == true)
            {
                mySampleTimer.Reset();
                std::cout << "Analog value: " << aIn.GetValue() << std::endl;
            }
        }
};

START_ROBOT_CLASS(Robot);
