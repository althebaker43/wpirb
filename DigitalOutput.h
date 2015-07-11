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

        /**
         * Examines and processes a packet if needed
         *
         * Digital output components never process incoming packets.
         */
        bool processPacket(
                const Packet& packet
                );

    private:

        /**
         * Output channel to drive digital signals on
         */
        uint32_t myChannel;

        /**
         * Packet to send to robot
         */
        Packet* myCurrentPacket;

        /**
         * Indicates if the pin has been configured yet
         */
        bool myIsPinConfigured;
};

#endif /* ifndef DIGITALOUTPUT_H */
