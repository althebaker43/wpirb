#ifndef PACKET_H
#define PACKET_H

#include <istream>
#include <stdint.h>
#include <vector>

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
            TYPE_AINPUT,    /**< Analog input packet */
            TYPE_PINCONFIG, /**< Pin configuration packet */
            TYPE_MDRIVE,    /**< Motor drive packet */

            // Response packets
            TYPE_ACK,           /**< Acknowledgement packet */
            TYPE_DVALUE,        /**< Digital value response packet */
            TYPE_AVALUE,        /**< Analog value response packet */
            TYPE_PINCONFIGINFO, /**< Pin configuration info response packet */
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
            BID_AINPUT =    0x04,
            BID_PINCONFIG = 0x05,
            BID_MDRIVE =    0x06,

            // Response packets
            BID_ACK =           0x82,
            BID_DVALUE =        0x81,
            BID_AVALUE =        0x83,
            BID_PINCONFIGINFO = 0x84,
        };

        /**
         * Enumeration of all pin directions
         */
        enum PinDirection
        {
            DIR_OUTPUT = 1,
            DIR_INPUT,
        };

        /**
         * Reads a packet from the given input stream
         *
         * A new packet is allocated on the heap and a pointer to it is returned.
         * Callers are responsible for freeing the consumed memory afterwards.
         */
        static Packet* Read(
                std::istream&   inputStream,
                std::string*    readData = NULL /**< Optional buffer to dump read binary data to */
                );

        /**
         * Destructor
         */
        virtual ~Packet(){};

        /**
         * Writes serialized binary data to output stream
         *
         * If this packet's contents were read from an input stream and it is
         * invalid, then the raw data read from the stream will be written
         * instead of a proper representation of a valid packet.
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
         * Constructor with pin
         */
        DigitalInputPacket(
                unsigned int    pin     /**< Pin to read from */
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
 * Analog input request class
 *
 * This packet requests an 10-bit analog value from the robot
 */
class AnalogInputPacket : public Packet
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
 * This packet requests that a pin be configured a certain way
 */
class PinConfigPacket : public Packet
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
         * Default constructor
         */
        MotorDrivePacket();

        /**
         * Constructor given motor information
         */
        MotorDrivePacket(
                Motor       motor,      /**< Motor to drive */
                uint8_t     speed,      /**< Absolute speed to drive motor to */
                Direction   direction   /**< Direction to drive motor in */
                );

        /**
         * Constructor given motor and single floating-point drive value
         *
         * This constructor automatically translates the given value to a valid
         * speed and direction.
         */
        MotorDrivePacket(
                Motor   motor,      /**< Motor to drive */
                double  driveVal    /**< Signed floating-point drive value */
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

        /**
         * Raw binary data read from input stream
         */
        std::vector<int> myBinaryData;
};

/**
 * Analog value response class
 *
 * This packet is a response from the robot containing an analog input value.
 * It should be expected after sending an AnalogInputPacket.
 */
class AnalogValuePacket : public Packet
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
 * Pin configuration response class
 *
 * This packet contains configuration information about a certain pin. This
 * packet type should be expected after sending a PinConfigPacket.
 */
class PinConfigInfoPacket : public Packet
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

#endif /* ifndef PACKET_H */
