#ifndef DIGITALINPUT_H
#define DIGITALINPUT_H

#include "Input.h"
#include "RedBotPacket.h"
#include "ConfigurableInterface.h"
#include <stdint.h>

// Forward declarations
class PinConfigPacket;

/**
 * Digital input request class
 *
 * This packet requests a digital value from the robot
 */
class DigitalInputPacket : public RedBotPacket
{
    public:

        /**
         * Default constructor
         */
        DigitalInputPacket();

        /**
         * Constructor with pin
         */
        DigitalInputPacket(
                unsigned int    pin     /**< Pin to read from */
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
         * Provides the pin to read from
         */
        unsigned int getPin() const;

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
         * Pin to read digital signal from
         */
        unsigned int myPin;
};

/**
 * Digital value response class
 *
 * This packet is a response from the robot containing a digital input value.
 * It should be expected after sending a DigitalInputPacket.
 */
class DigitalValuePacket : public RedBotPacket
{
    public:

        /**
         * Default constructor
         */
        DigitalValuePacket();

        /**
         * Constructor with pin and value
         */
        DigitalValuePacket(
                unsigned int    pin,    /**< Pin read from */
                bool            value   /**< Value detected on pin */
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
         * Provides the pin read from
         */
        unsigned int getPin() const;

        /**
         * Provides the value read from the pin
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
         * Pin read from
         */
        unsigned int myPin;

        /**
         * Value read from pin
         */
        bool myValue;

        /**
         * Indicates that this packet is valid
         */
        bool myIsValid;

        /**
         * Raw binary data read from input stream
         */
        std::vector<int> myBinaryData;
};

/**
 * Digital input class
 */
class DigitalInput :
    public Input<DigitalInputPacket, DigitalValuePacket, bool>,
    private ConfigurableInterface
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

        /**
         * Examines the packet and processes it if appropriate
         */
        bool processPacket(
                const Packet& packet
                );
};

#endif /* ifndef DIGITALINPUT_H */
