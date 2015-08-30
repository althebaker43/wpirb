
#include "TestUtils.h"
#include <sstream>


/**
 * Provides a message explaining the different between the two given packets
 */
static void ExplainDiffPackets(
        const char*     expectedString,
        const char*     actualString,
        std::string&    message         /**< String to store message in */
        );

/**
 * Writes the hexadecimal characters of the given binary packet
 */
static void WritePacketString(
        const char*     str,
        std::ostream&   outputStream
        );


PacketDiffExplainer::PacketDiffExplainer(
        const char* expectedString, 
        const char* actualString
        )
{
    ExplainDiffPackets(
            expectedString,
            actualString,
            myMessage
            );
}

const char*
PacketDiffExplainer::getMessage() const
{
    return myMessage.c_str();
}


void ExplainDiffPackets(
        const char*     expectedString,
        const char*     actualString,
        std::string&    message
        )
{
    std::ostringstream outputStream;

    outputStream << "Packet binary strings do not match" << std::endl;

    outputStream << "Expected: ";
    WritePacketString(expectedString, outputStream);
    outputStream << std::endl;

    outputStream << "Actual: ";
    WritePacketString(actualString, outputStream);
    outputStream << std::endl;

    message = outputStream.str();
}

void WritePacketString(
        const char*     str,
        std::ostream&   outputStream
        )
{
    outputStream << std::hex;

    size_t strLen = strlen(str);
    for(
            size_t pos = 0;
            pos < strLen;
            ++pos
       )
    {
        if (pos > 0)
        {
            outputStream << ' ';
        }

        outputStream << "0x" << (0xFF & ((unsigned int)str[pos]));
    }

    outputStream << std::dec;
}
