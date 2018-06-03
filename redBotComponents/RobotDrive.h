#ifndef ROBOTDRIVE_H
#define ROBOTDRIVE_H

#include "RedBotComponent.h"
#include "RedBotPacket.h"
#include <stdint.h>

class MotorDrivePacket;

namespace frc
{

class SpeedController;

/**
 * Handles common drive operations for a robot with two motors
 */
class DifferentialDrive : public RedBotComponent
{
    public:

        /**
         * Initializes the robot drive
         */
        DifferentialDrive(
		SpeedController& leftController,
                SpeedController& rightController
                );

        void SetExpiration(
                double expiration
                );

        /**
         * Drives the robot with the given magnitude and curve
	 *
	 * An optional argument disables squaring the magnitude and
	 * curve inputs before forwarding them to the speed
	 * controllers. Normally, this makes the robot less sensitive
	 * to input changes at low speeds.
         */
        void ArcadeDrive(
                double magnitude,
                double curve,
                bool squaredInputs = true
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

	SpeedController& getLeftController();

	SpeedController& getRightController();

    private:

	SpeedController& myLeftController;

	SpeedController& myRightController;
};

}; /* namespace frc */

#endif /* ifndef ROBOTDRIVE_H */
