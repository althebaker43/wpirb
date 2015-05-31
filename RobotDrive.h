#ifndef ROBOTDRIVE_H
#define ROBOTDRIVE_H

#include <stdint.h>
#include "Component.h"

// Forward declarations
class Joystick;
class Packet;
class MotorDrivePacket;

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
