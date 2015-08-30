#ifndef REDBOTPACKET_H
#define REDBOTPACKET_H

#include "Packet.h"

/**
 * Common base class for all packets used for RedBot communication
 */
class RedBotPacket : public Packet
{
    public:

        /**
         * Destructor
         */
        virtual ~RedBotPacket(){}

        /**
         * Inequality operator
         */
        bool operator!=(
                const Packet& packet
                ) const;

        /**
         * String conversion operator
         */
        virtual operator std::string() const;
};

/**
 * RedBot packet generator class
 */
class RedBotPacketGenerator : public PacketGenerator
{
    public:

        /**
         * Creates a new blank packet given a packet type byte
         */
        Packet* createPacket(
                unsigned char type
                );

        /**
         * Creates a ping packet
         */
        Packet* createPingPacket();
};

/**
 * Ping packet class
 */
class PingPacket : public RedBotPacket
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
 * Motor drive command class
 */
class MotorDrivePacket : public RedBotPacket
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
class AcknowledgePacket : public RedBotPacket
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

#endif /* ifndef REDBOTPACKET_H */
