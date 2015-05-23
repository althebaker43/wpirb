#ifndef ROBOTDRIVE_H
#define ROBOTDRIVE_H

#include <stdint.h>
#include "Component.h"

class Joystick;
class Packet;
class MotorDrivePacket;


class RobotDrive : public Component
{
    public:

        RobotDrive(
                uint32_t leftChannel,
                uint32_t rightChannel
                );

        void SetExpiration(
                double expiration
                );

        void ArcadeDrive(
                Joystick& joystick
                );

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
