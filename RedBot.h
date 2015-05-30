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
         * Enumeration of all possible statuses for the robot
         */
        enum Status
        {
            STATUS_DISCONNECTED,    /**< Unable to connect to robot */
            STATUS_UNRESPONSIVE,    /**< Robot is unresponsive */
            STATUS_INCOHERENT,      /**< Robot is not sending vaild packets */
            STATUS_GOOD             /**< Robot is operating normally */
        };

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
        virtual ~RedBot();

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

        /**
         * Attempts to resynchronize with the robot
         *
         * A byte stream that should clear the robot's communication buffers is
         * sent when this function is called.
         */
        void resync();

        /**
         * Indicates the current status of the robot
         */
        Status getStatus() const;

        /**
         * Provides the last serialized binary transaction
         */
        void getLastBinaryTransaction(
                std::list<std::string>& sentData,
                std::list<std::string>& receivedData
                ) const;

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
         * Transfers data packets with the robot
         *
         * One or several exchanges are carried out during a transfer.
         */
        void transferData();

        /**
         * Executes a single packet exchange with the robot
         */
        void exchangePackets(
                Packet*     requestPacket,  /**< Packet to send to robot */
                Packet*&    responsePacket  /**< Packet received from robot */
                );

        /**
         * Program that controls this robot
         */
        IterativeRobot* myProgram;

        /**
         * Indicates the current status of the robot
         */
        Status myStatus;

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

        /**
         * Binary data of packets sent in last transaction
         */
        std::list<std::string> myLastTransactionSentData;

        /**
         * Binary data of packets received in last transaction
         */
        std::list<std::string> myLastTransactionReceivedData;
};

#endif /* ifndef REDBOT_H */
