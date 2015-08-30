#ifndef PINCONFIG_H
#define PINCONFIG_H

#include "RedBotPacket.h"

/**
 * This packet requests that a pin be configured a certain way
 */
class PinConfigPacket : public RedBotPacket
{
    public:

        /**
         * Default constructor
         */
        PinConfigPacket();

        /**
         * Constructor with pin and direction
         */
        PinConfigPacket(
                unsigned int    pin,    /**< Pin to configure */
                PinDirection    dir     /**< Direction to set pin to */
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
         * Provides the pin to configure
         */
        unsigned int getPin() const;

        /**
         * Provides the direction to set the pin to
         */
        PinDirection getDirection() const;

    private:

        /**
         * Pin to configure
         */
        unsigned int myPin;

        /**
         * Direction to configure pin to
         */
        PinDirection myDirection;
};

/**
 * Pin configuration response class
 *
 * This packet contains configuration information about a certain pin. This
 * packet type should be expected after sending a PinConfigPacket.
 */
class PinConfigInfoPacket : public RedBotPacket
{
    public:

        /**
         * Default constructor
         */
        PinConfigInfoPacket();

        /**
         * Constructor with pin and direction
         */
        PinConfigInfoPacket(
                unsigned int    pin,    /**< Pin described in this packet */
                PinDirection    dir     /**< Direction pin is set to */
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
         * Provides the pin this packet describes
         */
        unsigned int getPin() const;

        /**
         * Provides the direction this pin is configured for
         */
        PinDirection getDirection() const;

    private:

        /**
         * Pin this packet describes
         */
        unsigned int myPin;

        /**
         * Direction the pin is set to
         */
        PinDirection myDirection;
};

#endif /* ifndef PINCONFIG_H */
