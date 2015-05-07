
#include "IOBuffer.h"


InputFileBuffer::InputFileBuffer(
        FILE* inputFile
        ) :
    myInputFile(inputFile),
    myIsHeaderRead(false),
    myIsPacketComplete(false)
{
}

InputFileBuffer::~InputFileBuffer()
{
}

bool
InputFileBuffer::read()
{
    if (isPacketComplete() == true)
    {
        return false;
    }

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
InputFileBuffer::isPacketComplete() const
{
    return myIsPacketComplete;
}

std::istream&
InputFileBuffer::getContents()
{
    return myByteBuffer;
}

void
InputFileBuffer::clear()
{
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
OutputFileBuffer::getContents()
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
