#include "WPILib.h"
#include <stdio.h>

class Robot : public IterativeRobot
{
    DigitalOutput myLED;
    Timer myBlinkTimer;
    uint32_t myLEDState;

    public:

        Robot() :
            myLED(9),
            myBlinkTimer(),
            myLEDState(0)
        {
        }

    private:

        void RobotInit()
        {
        }

        void DisabledInit()
        {
            myBlinkTimer.Start();
        }

        void DisabledPeriodic()
        {
            if (myBlinkTimer.HasPeriodPassed(0.5) == true)
            {
                myBlinkTimer.Reset();
                myLEDState = ((myLEDState == 1) ? 0 : 1);
                myLED.Set(myLEDState);
                printf("Program: switched LED state to: %d\n", myLEDState);
            }
        }
};

START_ROBOT_CLASS(Robot);
