#ifndef FIELDCONTROLSYSTEM_H
#define FIELDCONTROLSYSTEM_H

// Forward declaration
class RedBot;

/**
 * Encapsuales the Field Control System
 *
 * The Field Control System, or FCS, controls all active robots. Robots should
 * connect to the FCS immediately after a reset and listen to it for mode
 * changes.
 */
class FieldControlSystem
{
    public:

        /**
         * Enumeration of modes that the system can be in
         */
        enum Mode
        {
            MODE_DISABLED,
            MODE_AUTO,
            MODE_TELEOP,
            MODE_TEST,
            NUM_MODES
        };

        /**
         * Provides the singleton instance of this system
         */
        static FieldControlSystem* GetInstance();

        /**
         * Destroys the singleton instance of this system
         */
        static void DestroyInstance();

        /**
         * Registers a robot with this system
         *
         * Robots are required to be registered with this system in order to be
         * notified about mode changes.
         */
        void Register(
                RedBot* robot
                );

        /**
         * Instructs all robots to begin their autonomous modes
         */
        void EnableAutonomous();

        /**
         * Instructs all robots to begin their teleoperated modes
         */
        void EnableTeleop();

        /**
         * Instructs all robots to begin their test modes
         */
        void EnableTest();

        /**
         * Advances the field time by one cycle
         *
         * For each cycle, the periodic function of the current operating mode is run.
         */
        void Step();

        /**
         * Disables all robots
         */
        void Disable();

    private:

        /**
         * Singleton instance of this system
         */
        static FieldControlSystem* ourInstance;

        /**
         * Default constructor
         */
        FieldControlSystem();

        /**
         * Robot running on the field
         *
         * \todo Add support for multiple robots running at once
         */
        RedBot* myRobot;

        /**
         * Current mode of the system
         */
        Mode myMode;

        // Disabled copiers and destructor
        FieldControlSystem(const FieldControlSystem&);
        ~FieldControlSystem();
        FieldControlSystem& operator=(const FieldControlSystem&);
};

#endif /* ifndef FIELDCONTROLSYSTEM_H */
