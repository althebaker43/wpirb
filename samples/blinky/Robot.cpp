#include "WPILib.h"

class Robot : public IterativeRobot
{
    DigitalOutput myLED;
    Timer myBlinkTimer;
    bool myLEDState;

    public:

        Robot() :
            myLED(13),
            myBlinkTimer(),
            myLEDState(false)
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
                myLEDState = !myLEDState;
                myLED.Set(myLEDState);
            }
        }
};

START_ROBOT_CLASS(Robot);
