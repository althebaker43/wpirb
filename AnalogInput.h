#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include "Input.h"
#include <stdint.h>

#include "Packet.h"

/**
 * Analog input class
 */
class AnalogInput : public Input<AnalogInputPacket, AnalogValuePacket, uint16_t>
{
    public:

        /**
         * Constructor given digital channel
         */
        AnalogInput(
                uint32_t channel
                );

        /**
         * Destructor
         */
        ~AnalogInput();

        /**
         * Gets the current value from the input channel
         *
         * This returns a 10-bit value produced by the Analog-to-Digital
         * converter that the input pin is connected to on the robot.
         */
        uint16_t GetValue();

        /**
         * Provides the next available packet to send
         */
        Packet* getNextPacket();
};

#endif /* ifndef ANALOGINPUT_H */
