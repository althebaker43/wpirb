#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include "Input.h"
#include "RedBotPacket.h"
#include <stdint.h>

/**
 * Analog input request class
 *
 * This packet requests an 10-bit analog value from the robot
 */
class AnalogInputPacket : public RedBotPacket
{
    public:

        /**
         * Default constructor
         */
        AnalogInputPacket();

        /**
         * Constructor with pin
         */
        AnalogInputPacket(
                unsigned int    pin /**< Pin to read from */
                );

        /**
         * Writes serialized binary data to output stream
         */
        void write(
                std::ostream&
                ) const;

        /**
         * Generates an XML representation of this packet
         */
        void writeXML(
                std::ostream&
                ) const;

        /**
         * Reads serialized binary data from input stream
         */
        void read(
                std::istream&
                );

        /**
         * Indicates if this packet is valid or not
         */
        bool isValid () const;

        /**
         * Equality operator
         */
        bool operator==(
                const Packet&
                ) const;

        /**
         * Provides the type of this packet
         */
        Type getType() const;

        /**
         * Provides the pin to read from
         */
        unsigned int getPin() const;

    private:

        /**
         * Pin to read analog value from
         */
        unsigned int myPin;
};

/**
 * Analog value response class
 *
 * This packet is a response from the robot containing an analog input value.
 * It should be expected after sending an AnalogInputPacket.
 */
class AnalogValuePacket : public RedBotPacket
{
    public:

        /**
         * Default constructor
         */
        AnalogValuePacket();

        /**
         * Constructor with pin and value
         */
        AnalogValuePacket(
                unsigned int    pin,    /**< Pin read from */
                unsigned int    value   /**< Value detected on pin */
                );

        /**
         * Writes serialized binary data to output stream
         */
        void write(
                std::ostream&
                ) const;

        /**
         * Generates an XML representation of this packet
         */
        void writeXML(
                std::ostream&
                ) const;

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
         * Provides the type of this packet
         */
        Type getType() const;

        /**
         * Provides the pin read from
         */
        unsigned int getPin() const;

        /**
         * Provides the 10-bit value read from the pin
         */
        unsigned int getValue() const;

    private:

        /**
         * Pin read from
         */
        unsigned int myPin;

        /**
         * Analog value read from pin
         */
        unsigned int myValue;
};

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
