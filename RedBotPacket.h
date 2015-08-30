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
         * Destructor
         */
        virtual ~RedBotPacket(){}

        /**
         * Provides the type of packet
         */
        virtual Type getType() const = 0;

        /**
         * Indicates if this packet is an acknowledgement packet
         */
        bool isAcknowledge() const;

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

#endif /* ifndef REDBOTPACKET_H */
