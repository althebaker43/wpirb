#ifndef DIGITALOUTPUT_H
#define DIGITALOUTPUT_H

#include <stdint.h>
#include "Component.h"

/**
 * Digital output class
 */
class DigitalOutput : public Component
{
    public:

        /**
         * Constructor given digital channel
         */
        DigitalOutput(
                uint32_t channel
                );

        /**
         * Destructor
         */
        ~DigitalOutput();

        /**
         * Sets the output channel to the given digital value (0 or 1)
         */
        void Set(
                uint32_t value
                );

        /**
         * Provides the next available packet to send
         */
        Packet* getNextPacket();

    private:

        /**
         * Output channel to drive digital signals on
         */
        uint32_t myChannel;

        /**
         * Packet to send to robot
         */
        Packet* myCurrentPacket;
};

#endif /* ifndef DIGITALOUTPUT_H */
