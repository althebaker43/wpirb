
#include "Packet.h"
#include <sstream>


Packet*
Packet::Read(
        std::istream&       inputStream,
        PacketGenerator&    packetGen,
        std::string*        readData
        )
{
    if (inputStream.good() == false)
    {
        return NULL;
    }

    if (readData != NULL)
    {
        readData->clear();
    }

    std::string tmpDataBuf;

    unsigned char header;
    inputStream.get((char&)header);
    if (inputStream.good() == false)
    {
        return NULL;
    }
    tmpDataBuf.push_back(header);

    if (header == BINARY_BOUND)
    {
        Packet* packet = NULL;
        char packetType = '\0';
        inputStream.get(packetType);
        if (inputStream.good() == false)
        {
            return NULL;
        }
        tmpDataBuf.push_back(packetType);

        packet = packetGen.createPacket((unsigned char)packetType);

        if (packet != NULL)
        {
            inputStream >> *packet;

            if (readData != NULL)
            {
                // Dump read data for debugging
                std::ostringstream outputStream;
                outputStream << *packet;
                *readData = outputStream.str();
            }

            if (packet->isValid() == true)
            {
                return packet;
            }
            else
            {
                // Delete invalid packet
                delete packet;

                return NULL;
            }
        }
    }

    // Dump read data for debugging
    if (readData != NULL)
    {
        *readData = tmpDataBuf;
    }

    return NULL;
}


std::ostream&
operator<<(
        std::ostream&   outputStream,
        const Packet&   packet
        )
{
    packet.write(outputStream);
    return outputStream;
}

std::istream&
operator>>(
        std::istream&   inputStream,
        Packet&         packet
        )
{
    packet.read(inputStream);
    return inputStream;
}

