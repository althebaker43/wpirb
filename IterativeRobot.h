#ifndef ROBOTBASE_H
#define ROBOTBASE_H


class IterativeRobot
{
    public:

        virtual void RobotInit();

        virtual void AutonomousInit();

        virtual void AutonomousPeriodic();

        virtual void TeleopInit();

        virtual void TeleopPeriodic();

        virtual void TestPeriodic();
};


#endif /* ifndef ROBOTBASE_H */
