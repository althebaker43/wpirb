
#include "IOBuffer.h"
#include <poll.h>


InputFileBuffer::InputFileBuffer(
        FILE* inputFile
        ) :
    myInputFile(inputFile),
    myIsHeaderRead(false),
    myIsPacketComplete(false),
    myLastReadChar('\0')
{
}

InputFileBuffer::~InputFileBuffer()
{
}

bool
InputFileBuffer::readPacket()
{
    while (read() == true);

    return isPacketComplete();
}

bool
InputFileBuffer::read()
{
    if (isPacketComplete() == true)
    {
        return false;
    }

    // First attempt to read input
    int readVal = getc(myInputFile);
    if (readVal == EOF)
    {
        // If no input available yet, wait for a certain amount of time

        struct pollfd pollInfo;
        pollInfo.fd = fileno(myInputFile);
        pollInfo.events = POLLIN;

        int pollRetVal = poll(&pollInfo, 1, 1000);
        if (pollRetVal <= 0)
        {
            return false;
        }

        // Second attept to read input
        readVal = getc(myInputFile);
        if (readVal == EOF)
        {
            return false;
        }
    }

    char readChar = (char)readVal;
    if (readChar == '\xFF')
    {
        if (myIsHeaderRead == false)
        {
            myIsHeaderRead = true;
        }
        else
        {
            if (myLastReadChar == '\xFF')
            {
                // If back-to-back packet boundaries are found, then clear this
                // buffer and use the current byte as the header
                clear();
                myIsHeaderRead = true;
            }
            else
            {
                myIsPacketComplete = true;
            }
        }
    }

    myByteBuffer << readChar;
    myLastReadChar = readChar;

    return true;
}

bool
InputFileBuffer::isPacketComplete() const
{
    return myIsPacketComplete;
}

std::istream&
InputFileBuffer::getInputStream()
{
    return myByteBuffer;
}

void
InputFileBuffer::clear()
{
    myByteBuffer.clear();
    myByteBuffer.str("");
    myIsPacketComplete = false;
    myIsHeaderRead = false;
}


OutputFileBuffer::OutputFileBuffer(
        FILE* outputFile
        ) :
    myOutputFile(outputFile),
    myByteBuffer(
            std::ios::in |
            std::ios::out |
            std::ios::binary
            ),
    myIsHeaderWritten(false),
    myIsPacketComplete(false)
{
}

OutputFileBuffer::~OutputFileBuffer()
{
}

bool
OutputFileBuffer::writePacket()
{
    while (write() == true);

    fflush(myOutputFile);

    return isPacketComplete();
}

bool
OutputFileBuffer::write()
{
    if (isPacketComplete() == true)
    {
        return false;
    }

    myByteBuffer.flush();

    int writeVal = myByteBuffer.get();
    if (myByteBuffer.good() == false)
    {
        return false;
    }

    if (writeVal == 0xFF)
    {
        if (myIsHeaderWritten == false)
        {
            myIsHeaderWritten = true;
        }
        else
        {
            myIsPacketComplete = true;
        }
    }

    if (putc(writeVal, myOutputFile) == EOF)
    {
        return false;
    }

    return true;
}

bool
OutputFileBuffer::isPacketComplete() const
{
    return myIsPacketComplete;
}

std::ostream&
OutputFileBuffer::getOutputStream()
{
    return myByteBuffer;
}

void
OutputFileBuffer::clear()
{
    myByteBuffer.clear();
    myByteBuffer.str("");
    myIsPacketComplete = false;
    myIsHeaderWritten = false;
}

void
OutputFileBuffer::resync()
{
    fprintf(
            myOutputFile,
            "\xFF\xFF\xFF\xFF\xFF"
           );
    fflush(myOutputFile);
}
