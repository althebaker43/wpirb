
#include "IOBuffer.h"


InputBuffer::InputBuffer(
        FILE* inputFile
        ) :
    myInputFile(inputFile),
    myIsHeaderRead(false),
    myIsPacketComplete(false)
{
}

InputBuffer::~InputBuffer()
{
}

bool
InputBuffer::read()
{
    int readVal = getc(myInputFile);

    if (readVal == EOF)
    {
        return false;
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
            myIsPacketComplete = true;
        }
    }

    myByteBuffer << readChar;

    return true;
}

bool
InputBuffer::isPacketComplete() const
{
    return myIsPacketComplete;
}

std::istream&
InputBuffer::getContents()
{
    return myByteBuffer;
}


OutputBuffer::OutputBuffer(
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

OutputBuffer::~OutputBuffer()
{
}

bool
OutputBuffer::write()
{
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
OutputBuffer::isPacketComplete() const
{
    return myIsPacketComplete;
}

std::ostream&
OutputBuffer::getContents()
{
    return myByteBuffer;
}
