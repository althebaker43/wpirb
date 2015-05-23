#ifndef PACKET_H
#define PACKET_H

#include <istream>
#include <stdint.h>

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
            TYPE_MDRIVE,    /**< Motor drive packet */

            // Response packets
            TYPE_ACK,       /**< Acknowledgement packet */
            TYPE_DVALUE,    /**< Digital value response packet */
        };

        /**
         * Packet binary ID enumerations
         *
         * These IDs are encoded into the serialized data stream.
         */
        enum BinaryID
        {
            // Request packets
            BID_PING =      0x01,
            BID_DOUTPUT =   0x02,
            BID_DINPUT =    0x03,

            // Response packets
            BID_ACK =       0x82,
            BID_DVALUE =    0x81,
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
         * Generates an XML representation of this packet
         */
        virtual void writeXML(
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
         * Default constructor
         */
        DigitalInputPacket();

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
 * Motor drive command class
 */
class MotorDrivePacket : public Packet
{
    public:

        /**
         * Motor enumeration
         */
        enum Motor
        {
            MOTOR_LEFT,
            MOTOR_RIGHT
        };

        /**
         * Motor direction enumeration
         */
        enum Direction
        {
            DIR_FORWARD,
            DIR_BACKWARD
        };

        /**
         * Constructor given motor information
         */
        MotorDrivePacket(
                Motor       motor,      /**< Motor to drive */
                uint8_t     speed,      /**< Absolute speed to drive motor to */
                Direction   direction   /**< Direction to drive motor in */
                );

        /**
         * Destructor
         */
        ~MotorDrivePacket();

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
         *
         * A Packet can be invalid if it tried to read from a malformed byte
         * stream.
         */
        bool isValid() const;

        /**
         * Equality operator
         */
        bool operator==(
                const Packet&
                ) const;

        /**
         * Provides the type of packet
         */
        Type getType() const;

        /**
         * Indicates which motor to drive
         */
        Motor getMotor() const;

        /**
         * Provides absolute speed to drive motor to
         */
        uint8_t getSpeed() const;

        /**
         * Indicates which way the motor will be driven
         */
        Direction getDirection() const;

    private:

        /**
         * Motor to drive
         */
        Motor myMotor;

        /**
         * Speed to drive motor to
         */
        uint8_t mySpeed;

        /**
         * Direction to drive motor in
         */
        Direction myDirection;
};

/**
 * Acknowledgement response class
 */
class AcknowledgePacket : public Packet
{
    public:

        /**
         * Default constructor
         */
        AcknowledgePacket();

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
         * Provides the type of packet
         */
        Type getType() const;

    private:

        /**
         * Indicates if this packet is valid or not
         */
        bool myIsValid;
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

        /**
         * Indicates that this packet is valid
         */
        bool myIsValid;
};

#endif /* ifndef PACKET_H */
