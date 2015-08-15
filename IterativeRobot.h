#ifndef ROBOTBASE_H
#define ROBOTBASE_H

/**
 * Robot base class whose functions iterate once per cycle
 *
 * All implementations of the initialzation and periodic functions in this
 * abstract class are empty.
 */
class IterativeRobot
{
    public:

        /**
         * Destructor
         */
        virtual ~IterativeRobot(){}

        /**
         * Global initialization function
         *
         * This function is called only once at the beginning of this program's
         * run
         */
        virtual void RobotInit();

        /**
         * Disabled mode initialization
         *
         * This function is called once every time the robot switches to the
         * 'disabled' mode
         */
        virtual void DisabledInit();

        /**
         * Disabled mode periodic function
         *
         * This function is called once per cycle while the robot is in the
         * 'disabled' mode. Actuators should not be operated while in the
         * 'disabled' mode.
         */
        virtual void DisabledPeriodic();

        /**
         * Autonomous mode initialization
         *
         * This function is called once every time the robot switches to the
         * 'autonomous' mode
         */
        virtual void AutonomousInit();

        /**
         * Autonomous mode periodic function
         *
         * This function is called once per cycle while the robot is in the
         * 'autonomous' mode. Human operator input should not be used while in
         * this mode.
         */
        virtual void AutonomousPeriodic();

        /**
         * Teleoperated mode initialization
         *
         * This function is called once every time the robot switches to the
         * 'teleoperated' mode.
         */
        virtual void TeleopInit();

        /**
         * Teleoperated mode periodic function
         *
         * This function is called once per cycle while the robot is in the
         * 'teleoperated' mode.
         */
        virtual void TeleopPeriodic();

        /**
         * Test mode initialization
         *
         * This function is called once every time the robot switches to the
         * 'test' mode
         */
        virtual void TestInit();

        /**
         * Test mode periodic function
         *
         * This function is called once per cycle while the robot is in the
         * 'test' mode.
         */
        virtual void TestPeriodic();
};


#endif /* ifndef ROBOTBASE_H */
