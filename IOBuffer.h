#ifndef IOBUFFER_H
#define IOBUFFER_H

#include <stdio.h>
#include <sstream>


/**
 * Reads and buffers packet from a file or device
 */
class InputBuffer
{
    public:

        /**
         * Default constructor
         */
        InputBuffer();

        /**
         * Constructor with file stream to read packets from
         */
        InputBuffer(
                FILE* inputFile
                );

        /**
         * Assignment operator
         */
        InputBuffer& operator=(
                const InputBuffer& source
                );

        /**
         * Destructor
         *
         * The file stream is not closed when this object is destroyed.
         */
        ~InputBuffer();

        /**
         * Reads a single byte from the input file stream
         *
         * \return True if read operation was successful, false otherwise or if
         * a complete packet has already been read in.
         */
        bool read();

        /**
         * Indicates if a complete packet is currently buffered.
         */
        bool isPacketComplete() const;

        /**
         * Provides an input stream to read complete packets from
         */
        std::istream& getContents();

        /**
         * Clears the contents of this buffer
         */
        void clear();

    private:

        /**
         * File stream to read packets from
         */
        FILE* myInputFile;

        /**
         * Buffer for packet bytes
         */
        std::stringstream myByteBuffer;

        /**
         * Indicates if the packet header byte has been read yet
         */
        bool myIsHeaderRead;

        /**
         * Indicates if complete packet is currently buffered
         */
        bool myIsPacketComplete;
};

/**
 * Buffers and writes packets to a file stream
 */
class OutputBuffer
{
    public:

        /**
         * Default constructor
         */
        OutputBuffer();

        /**
         * Construtor with file stream to write packets to
         */
        OutputBuffer(
                FILE* outputFile
                );

        /**
         * Assignment operator
         */
        OutputBuffer& operator=(
                const OutputBuffer& source
                );

        /**
         * Destructor
         *
         * The file stream is not closed when this object is destroyed.
         */
        ~OutputBuffer();

        /**
         * Writes a single byte to the output file stream
         *
         * \return True if the write operation was successful, false otherwise
         * or if a complete packet has already been written out.
         */
        bool write();

        /**
         * Indicates if a complete packet has been written out
         */
        bool isPacketComplete() const;

        /**
         * Provides an output stream to write complete packets to
         */
        std::ostream& getContents();

        /**
         * Clears the contents of this buffer
         */
        void clear();

    private:

        /**
         * File stream to write packets to
         */
        FILE* myOutputFile;

        /**
         * Buffer for packet bytes
         */
        std::stringstream myByteBuffer;

        /**
         * Indicates if the packet header byte has been written yet
         */
        bool myIsHeaderWritten;

        /**
         * Indicates if complete packet has been written
         */
        bool myIsPacketComplete;
};

#endif /* ifndef IOBUFFER_H */
