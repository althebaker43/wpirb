#ifndef DIGITALOUTPUT_H
#define DIGITALOUTPUT_H

#include <stdint.h>
#include "Component.h"
#include "RedBotPacket.h"
#include "ConfigurableInterface.h"

/**
 * Digital output command class
 *
 * This packet directs the robot to output a digital value to a pin
 */
class DigitalOutputPacket : public RedBotPacket
{
    public:

        /**
         * Default constructor
         */
        DigitalOutputPacket();

        /**
         * Constructor with pin and value
         */
        DigitalOutputPacket(
                unsigned int    pin,    /**< Pin to output to */
                bool            value   /**< Value to output */
                );

        /**
         * Reads serialized binary data from input stream
         */
        void read(
                std::istream&
                );

        /**
         * Indicates if this packet is valid or not
         */
        bool isValid() const;

        /**
         * Equality operator
         */
        bool operator==(
                const Packet&
                ) const;

        /**
         * Provides the number of the pin to output to
         */
        unsigned int getPin() const;

        /**
         * Provides the value to output
         */
        bool getValue() const;

    private:

        /**
         * Writes binary packet contents to output stream
         */
        void writeContents(
                std::ostream&
                ) const;

        /**
         * Provides elements to include in the XML representation
         */
        void getXMLElements(
                XMLElements& elements
                ) const;

        /**
         * Pin to output to
         */
        unsigned int myPin;

        /**
         * Digital value to output to pin
         */
        bool myValue;
};

/**
 * Digital output class
 */
class DigitalOutput :
    public Component,
    private ConfigurableInterface
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
};

#endif /* ifndef DIGITALOUTPUT_H */
