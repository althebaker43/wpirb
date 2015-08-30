#ifndef ROBOTDRIVE_H
#define ROBOTDRIVE_H

#include "Component.h"
#include "RedBotPacket.h"
#include <stdint.h>

// Forward declarations
class Joystick;

/**
 * Motor drive command class
 */
class MotorDrivePacket : public RedBotPacket
{
    public:

        /**
         * Motor enumeration
         */
        enum Motor
        {
            MOTOR_LEFT,
            MOTOR_RIGHT
        };

        /**
         * Motor direction enumeration
         */
        enum Direction
        {
            DIR_FORWARD,
            DIR_BACKWARD
        };

        /**
         * Default constructor
         */
        MotorDrivePacket();

        /**
         * Constructor given motor information
         */
        MotorDrivePacket(
                Motor       motor,      /**< Motor to drive */
                uint8_t     speed,      /**< Absolute speed to drive motor to */
                Direction   direction   /**< Direction to drive motor in */
                );

        /**
         * Constructor given motor and single floating-point drive value
         *
         * This constructor automatically translates the given value to a valid
         * speed and direction.
         */
        MotorDrivePacket(
                Motor   motor,      /**< Motor to drive */
                double  driveVal    /**< Signed floating-point drive value */
                );

        /**
         * Destructor
         */
        ~MotorDrivePacket();

        /**
         * Writes serialized binary data to output stream
         */
        void write(
                std::ostream&
                ) const;

        /**
         * Generates an XML representation of this packet
         */
        void writeXML(
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
         *
         * A Packet can be invalid if it tried to read from a malformed byte
         * stream.
         */
        bool isValid() const;

        /**
         * Equality operator
         */
        bool operator==(
                const Packet&
                ) const;

        /**
         * Provides the type of packet
         */
        Type getType() const;

        /**
         * Indicates which motor to drive
         */
        Motor getMotor() const;

        /**
         * Provides absolute speed to drive motor to
         */
        uint8_t getSpeed() const;

        /**
         * Indicates which way the motor will be driven
         */
        Direction getDirection() const;

    private:

        /**
         * Motor to drive
         */
        Motor myMotor;

        /**
         * Speed to drive motor to
         */
        uint8_t mySpeed;

        /**
         * Direction to drive motor in
         */
        Direction myDirection;
};

/**
 * Handles common drive operations for a robot with two motors
 */
class RobotDrive : public Component
{
    public:

        /**
         * Initializes the robot drive
         */
        RobotDrive(
                uint32_t leftChannel,
                uint32_t rightChannel
                );

        void SetExpiration(
                double expiration
                );

        /**
         * Drives the robot arcade style using the given joystick
         *
         * This function uses the Y-axis from the joystick as the magnitude and
         * the X-axis as the curve for the Drive function.
         */
        void ArcadeDrive(
                Joystick& joystick
                );

        /**
         * Drives the robot with the given magnitude and curve
         */
        void Drive(
                double magnitude,
                double curve
                );

        /**
         * Provides the next packet to send to the robot
         */
        Packet* getNextPacket();

        /**
         * Processes a packet received from the robot
         */
        bool processPacket(
                const Packet& packet
                );

    private:

        /**
         * Next packet to send to robot to drive left motor
         */
        MotorDrivePacket* myCurrentLMotorPacket;

        /**
         * Next packet to send to robot to drive right motor
         */
        MotorDrivePacket* myCurrentRMotorPacket;
};


#endif /* ifndef ROBOTDRIVE_H */
