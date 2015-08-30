#ifndef PACKET_H
#define PACKET_H

#include <istream>
#include <stdint.h>
#include <vector>

// Forward declarations
class PacketGenerator;

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
                std::istream&       inputStream,
                PacketGenerator&    packetGen,      /**< Packet generator to use to create new packets */
                std::string*        readData = NULL /**< Optional buffer to dump read binary data to */
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
        virtual bool operator!=(
                const Packet&
                ) const = 0;

        /**
         * Provides the type of packet
         */
        virtual Type getType() const = 0;

        /**
         * String conversion operator
         */
        virtual operator std::string() const = 0;
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
 * Packet generator interface
 *
 * Classes with this interface provide blank packets according to the type
 * field extracted from a binary packet stream.
 */
class PacketGenerator
{
    public:

        /**
         * Destructor
         */
        virtual ~PacketGenerator(){}

        /**
         * Creates a new blank packet given a packet type byte
         *
         * \return Pointer to new packet if given type is valid, NULL otherwise
         */
        virtual Packet* createPacket(
                unsigned char type
                ) = 0;

        /**
         * Creates a ping packet
         */
        virtual Packet* createPingPacket() = 0;
};

#endif /* ifndef PACKET_H */
