#ifndef DIGITALINPUT_H
#define DIGITALINPUT_H

#include "Component.h"
#include <stdint.h>

class DigitalInputPacket;

/**
 * Digital input class
 */
class DigitalInput : public Component
{
    public:

        /**
         * Constructor given digital channel
         */
        DigitalInput(
                uint32_t channel
                );

        /**
         * Destructor
         */
        ~DigitalInput();

        /**
         * Gets the current value from the input channel
         *
         * \return True if the input is high, false if it is low
         */
        bool Get();

        /**
         * Provides the next available packet to send
         */
        Packet* getNextPacket();

        /**
         * Examines and processes a packet if needed
         *
         * Digital input components only process instances of DigitalValuePacket.
         */
        bool processPacket(
                const Packet& packet
                );

    private:

        /**
         * Number of cycles to wait for reply packet before calling time-out
         */
        const static unsigned int TIMEOUT_THRESH = 5;

        /**
         * Channel to read digital signal from
         */
        uint32_t myChannel;

        /**
         * Last digital value read from pin
         */
        bool myValue;

        /**
         * Indicates if the pin has been configured yet
         */
        bool myIsPinConfigured;

        /**
         * Next packet to send to robot
         */
        DigitalInputPacket* myOutgoingPacket;

        /**
         * Counter value used to detect time-outs for packet replies
         */
        unsigned int myTimeoutCounter;
};

#endif /* ifndef DIGITALINPUT_H */
