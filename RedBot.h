#ifndef REDBOT_H
#define REDBOT_H

#include "FieldControlSystem.h"
#include <stdio.h>
#include <termios.h>

// Forward declaration
class IterativeRobot;

/**
 * Handler for all communications with a robot
 *
 * Objects of this class act as handlers for a robot program and a serial link
 * to a physical robot on the field that the program controls.
 */
class RedBot
{
    public:

        /**
         * Constructor given program and serial device
         *
         * This attempts to open the named serial port to establish
         * communication with the robot.
         */
        RedBot(
                IterativeRobot* program,
                const char*     deviceName
              );

        /**
         * Destructor
         *
         * This closes the serial port connected to the robot.
         */
        ~RedBot();

        /**
         * Indicates if the robot is connected and responsive
         */
        virtual bool isConnected() const;

        /**
         * Launches initialization process for the given mode
         */
        void modeInit(
                FieldControlSystem::Mode
                );

        /**
         * Launches periodic function for the given mode
         */
        void modePeriodic(
                FieldControlSystem::Mode
                );

    private:

        /**
         * Baud rate enumerator for robot connection
         */
        static const speed_t OUR_DEV_SPEED = B9600;

        /**
         * Program that controls this robot
         */
        IterativeRobot* myProgram;

        /**
         * Flag for robot connection status
         */
        bool myIsConnected;

        /**
         * File stream object for robot connection
         */
        FILE* myDevice;
};

#endif /* ifndef REDBOT_H */
