#ifndef PACKET_H
#define PACKET_H

#include <istream>

/**
 * Packet base class definition
 */
class Packet
{
    public:

        /**
         * Packet type enumerations
         */
        enum Type
        {
            TYPE_UNKNOWN = 0,

            // Request packets
            TYPE_PING,      /**< Ping packet */
            TYPE_DOUTPUT,   /**< Digital output packet */
            TYPE_DINPUT,    /**< Digital input packet */

            // Response packets
            TYPE_DVALUE,    /**< Digital value response packet */
        };

        /**
         * Reads a packet from the given input stream
         *
         * A new packet is allocated on the heap and a pointer to it is returned.
         * Callers are responsible for freeing the consumed memory afterwards.
         */
        static Packet* Read(
                std::istream&
                );

        /**
         * Destructor
         */
        virtual ~Packet(){};

        /**
         * Writes serialized binary data to output stream
         */
        virtual void write(
                std::ostream&
                ) const = 0;

        /**
         * Reads serialized binary data from input stream
         */
        virtual void read(
                std::istream&
                ) = 0;

        /**
         * Indicates if this packet is valid or not
         *
         * A Packet can be invalid if it tried to read from a malformed byte
         * stream.
         */
        virtual bool isValid() const = 0;

        /**
         * Equality operator
         */
        virtual bool operator==(
                const Packet&
                ) const = 0;

        /**
         * Inequality operator
         */
        bool operator!=(
                const Packet&
                ) const;

        /**
         * Provides the type of packet
         */
        virtual Type getType() const = 0;

        /**
         * String conversion operator
         */
        virtual operator std::string() const;
};

/**
 * Output streaming operator for packets
 */
std::ostream&
operator<<(
        std::ostream&   outputStream,
        const Packet&   packet
        );

/**
 * Input streaming operator for packets
 */
std::istream&
operator>>(
        std::istream&   inputStream,
        Packet&         packet
        );

/**
 * Ping packet class
 */
class PingPacket : public Packet
{
    public:

        /**
         * Default constructor
         */
        PingPacket();

        /**
         * Writes serialized binary data to output stream
         */
        void write(
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

    private:

        /**
         * Indicates if this packet is valid or not
         */
        bool myIsValid;
};

/**
 * Digital output command class
 *
 * This packet directs the robot to output a digital value to a pin
 */
class DigitalOutputPacket : public Packet
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
         * Writes serialized binary data to output stream
         */
        void write(
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
         * Provides the number of the pin to output to
         */
        unsigned int getPin() const;

        /**
         * Provides the value to output
         */
        bool getValue() const;

    private:

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
 * Digital input request class
 *
 * This packet requests a digital value from the robot
 */
class DigitalInputPacket : public Packet
{
    public:

        /**
         * Constructor with pin and value
         */
        DigitalInputPacket(
                unsigned int    pin     /**< Pin to output to */
                );

        /**
         * Writes serialized binary data to output stream
         */
        void write(
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
         * Provides the pin to read from
         */
        unsigned int getPin() const;

    private:

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
class DigitalValuePacket : public Packet
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
         * Writes serialized binary data to output stream
         */
        void write(
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
         * Provides the value read from the pin
         */
        bool getValue() const;

    private:

        /**
         * Pin read from
         */
        unsigned int myPin;

        /**
         * Value read from pin
         */
        bool myValue;
};

#endif /* ifndef PACKET_H */
