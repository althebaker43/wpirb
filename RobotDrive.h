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

        void ArcadeDrive(
                Joystick*
                );
};


#endif /* ifndef ROBOTDRIVE_H */
