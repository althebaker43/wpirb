#ifndef DIGITALINPUT_H
#define DIGITALINPUT_H

#include "Input.h"
#include <stdint.h>

#include "RedBotPacket.h"

/**
 * Digital input class
 */
class DigitalInput : public Input<DigitalInputPacket, DigitalValuePacket, bool>
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
         * Provides the next available packet to send
         */
        Packet* getNextPacket();

    private:

        /**
         * Indicates if the pin has been configured yet
         */
        bool myIsPinConfigured;
};

#endif /* ifndef DIGITALINPUT_H */
