#ifndef IOBUFFER_H
#define IOBUFFER_H

#include <stdio.h>
#include <sstream>


/**
 * Common interface for input packet buffers
 */
class InputBuffer
{
    public:

        /**
         * Destructor
         */
        virtual ~InputBuffer(){};

        /**
         * Reads in a single byte
         *
         * \return True if read operation was successful, false otherwise or if
         * a complete packet has already been read in.
         */
        virtual bool read() = 0;

        /**
         * Indicates if a complete packet is currently buffered
         */
        virtual bool isPacketComplete() const = 0;

        /**
         * Provides an input stream to read complete packets from
         */
        virtual std::istream& getContents() = 0;

        /**
         * Clears the contents of this buffer
         */
        virtual void clear() = 0;
};

/**
 * Common interface for output packet buffers
 */
class OutputBuffer
{
    public:

        /**
         * Destructor
         */
        virtual ~OutputBuffer(){};

        /**
         * Writes a single byte to the output file stream
         *
         * \return True if the write operation was successful, false otherwise
         * or if a complete packet has already been written out.
         */
        virtual bool write() = 0;

        /**
         * Indicates if a complete packet has been written out
         */
        virtual bool isPacketComplete() const = 0;

        /**
         * Provides an output stream to write complete packets to
         */
        virtual std::ostream& getContents() = 0;

        /**
         * Clears the contents of this buffer
         */
        virtual void clear() = 0;
};

/**
 * Reads and buffers packets from a file or device
 */
class InputFileBuffer : public InputBuffer
{
    public:

        /**
         * Constructor with file stream to read packets from
         */
        InputFileBuffer(
                FILE* inputFile
                );

        /**
         * Destructor
         *
         * The file stream is not closed when this object is destroyed.
         */
        ~InputFileBuffer();

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
class OutputFileBuffer : public OutputBuffer
{
    public:

        /**
         * Construtor with file stream to write packets to
         */
        OutputFileBuffer(
                FILE* outputFile
                );

        /**
         * Destructor
         *
         * The file stream is not closed when this object is destroyed.
         */
        ~OutputFileBuffer();

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
