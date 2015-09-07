#ifndef PINCONFIG_H
#define PINCONFIG_H

#include "Component.h"
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
         * Provides the pin to configure
         */
        unsigned int getPin() const;

        /**
         * Provides the direction to set the pin to
         */
        PinDirection getDirection() const;

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
         * Provides the pin this packet describes
         */
        unsigned int getPin() const;

        /**
         * Provides the direction this pin is configured for
         */
        PinDirection getDirection() const;

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
         * Pin this packet describes
         */
        unsigned int myPin;

        /**
         * Direction the pin is set to
         */
        PinDirection myDirection;
};

/**
 * Base class for configurable I/O interfaces
 *
 * Components of this type need to configure their hardware first before using
 * them for I/O.
 */
class ConfigurableInterface
{
    public:

        /**
         * Constructor
         */
        ConfigurableInterface(
                unsigned int                pin,    /**< Pin to configure */
                RedBotPacket::PinDirection  dir     /**< Direction to set pin */
                );

        /**
         * Destructor
         */
        virtual ~ConfigurableInterface(){}

    protected:

        /**
         * Provides the next configuration packet if available
         */
        Packet* getNextConfigPacket();

        /**
         * Processes the given configuration packet if appropriate
         *
         * \note A true return value from this function does not necessarily
         * mean that the pin was successfully configured, only that the given
         * packet was successfully processed.
         *
         * \return True if the given packet was successfully parsed, false
         * otherwise.
         */
        bool processConfigInfoPacket(
                const Packet& packet
                );

        /**
         * Indicates if configuration is complete
         *
         * Instances are initialized as not being configured. After
         * communicating configuration information to the robot and verifying
         * its responses, they become configured.
         */
        bool isConfigured() const;

    private:

        /**
         * Number of pin that needs to be configured
         */
        const unsigned int myPin;

        /**
         * Direction that pin should be set to
         */
        const RedBotPacket::PinDirection myDirection;

        /**
         * Indicates if the pin has been configured yet
         */
        bool myIsPinConfigured;

        /**
         * Pin configuration packet sent for this cycle
         */
        PinConfigPacket* myConfigPacket;
};

#endif /* ifndef PINCONFIG_H */
