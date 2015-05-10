#ifndef REDBOT_H
#define REDBOT_H

#include "FieldControlSystem.h"
#include "IOBuffer.h"
#include <stdio.h>
#include <termios.h>
#include <list>
#include <queue>

// Forward declarations
class IterativeRobot;
class Component;
class Packet;

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
         * Registers a component used in a program with a robot object
         *
         * This is done during program initialization so that packets can be
         * exchanged between the robot and the different components.
         */
        static void RegisterComponent(
                Component* component
                );

        /**
         * Clears the registered components awaiting ownership
         */
        static void ClearRegisteredComponents();

        /**
         * Constructor given program and serial device name
         *
         * This attempts to open the named serial port to establish
         * communication with the robot.
         */
        RedBot(
                IterativeRobot* program,
                const char*     deviceName
              );

        /**
         * Constructor given program and open serial device
         *
         * This makes this robot object use the given file stream for packet
         * I/O.
         */
        RedBot(
                IterativeRobot* program,
                FILE*           device
              );

        /**
         * Constructor given program and I/O buffers
         *
         * This makes this robot object use the given buffers for packet I/O.
         */
        RedBot(
                IterativeRobot* program,
                InputBuffer*    inputBuffer,
                OutputBuffer*   outputBuffer
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
         * Component collection type
         */
        typedef std::list<Component*> Components;

        /**
         * Components used in last-initialized robot program
         *
         * After the program has finished being constructed, the robot instance
         * that owns the program should grab all of the components in this
         * collection to use for packet communication and then clear it.
         */
        static Components ourCurrentComponents;

        /**
         * Baud rate enumerator for robot connection
         */
        static const speed_t OUR_DEV_SPEED = B9600;

        /**
         * Exchanges data packets with the robot
         */
        void transferData();

        /**
         * Program that controls this robot
         */
        IterativeRobot* myProgram;

        /**
         * Flag for robot connection status
         */
        bool myIsConnected;

        /**
         * Indicates if this object is using externally provided IO buffers
         *
         * In the event that external I/O buffers are being used, they will not
         * be deleted when this object is deleted.
         */
        bool myIsUsingExternalBuffers;

        /**
         * File stream object for robot connection
         */
        FILE* myDevice;

        /**
         * Components used in the program
         */
        Components myComponents;

        /**
         * Buffer for incoming communication to robot
         */
        InputBuffer* myInputBuffer;

        /**
         * Buffer for outgoing communication to robot
         */
        OutputBuffer* myOutputBuffer;

        /**
         * Incoming packets from robot
         */
        std::queue<Packet*> myIncomingPackets;
};

#endif /* ifndef REDBOT_H */
