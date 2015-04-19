#ifndef ROBOTDRIVE_H
#define ROBOTDRIVE_H

#include <stdint.h>

class Joystick;


class RobotDrive
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
                double mangitude,
                double curve
                );
};


#endif /* ifndef ROBOTDRIVE_H */
