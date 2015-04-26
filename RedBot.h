#ifndef REDBOT_H
#define REDBOT_H

#include "FieldControlSystem.h"
#include <stdio.h>
#include <termios.h>
#include <ostream>
#include <istream>

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

/**
 * Packet base class definition
 */
class Packet
{
    public:

        /**
         * Packet type enumerations
         */
        enum Type
        {
            TYPE_UNKNOWN = 0,
            TYPE_PING,
        };

        /**
         * Reads a packet from the given input stream
         *
         * A new packet is allocated on the heap and a pointer to it is returned.
         * Callers are responsible for freeing the consumed memory afterwards.
         */
        static Packet* Read(
                std::istream&
                );

        /**
         * Destructor
         */
        virtual ~Packet(){};

        /**
         * Writes serialized binary data to output stream
         */
        virtual void write(
                std::ostream&
                ) const = 0;

        /**
         * Reads serialized binary data from input stream
         */
        virtual void read(
                std::istream&
                ) = 0;

        /**
         * Indicates if this packet is valid or not
         *
         * A Packet can be invalid if it tried to read from a malformed byte
         * stream.
         */
        virtual bool isValid() const = 0;

        /**
         * Equality operator
         */
        virtual bool operator==(
                const Packet&
                ) const = 0;

        /**
         * Inequality operator
         */
        bool operator!=(
                const Packet&
                ) const;

        /**
         * Provides the type of packet
         */
        virtual Type getType() const = 0;

        /**
         * String conversion operator
         */
        virtual operator std::string() const = 0;
};

/**
 * Output streaming operator for packets
 */
std::ostream&
operator<<(
        std::ostream&   outputStream,
        const Packet&   packet
        );

/**
 * Input streaming operator for packets
 */
std::istream&
operator>>(
        std::istream&   inputStream,
        Packet&         packet
        );

/**
 * Ping packet class
 */
class PingPacket : public Packet
{
    public:

        /**
         * Default constructor
         */
        PingPacket();

        /**
         * Writes serialized binary data to output stream
         */
        void write(
                std::ostream&
                ) const;

        /**
         * Reads serialized binary data from input stream
         */
        void read(
                std::istream&
                );

        /**
         * Indicates if this packet is valid or not
         */
        bool isValid() const;

        /**
         * Equality operator
         */
        bool operator==(
                const Packet&
                ) const;

        /**
         * Provides the type of this packet
         */
        Type getType() const;

        /**
         * String conversion operator
         */
        operator std::string() const;

    private:

        /**
         * Indicates if this packet is valid or not
         */
        bool myIsValid;
};

#endif /* ifndef REDBOT_H */
