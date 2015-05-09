#ifndef TESTREDBOT_H
#define TESTREDBOT_H

#include "IterativeRobot.h"
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "IOBuffer.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <stdio.h>

class MockInputBuffer : public InputBuffer
{
    public:

        MockInputBuffer()
        {
        }

        ~MockInputBuffer()
        {
        }

        bool read()
        {
            return false;
        }

        bool isPacketComplete() const
        {
            return false;
        }

        std::istream& getInputStream()
        {
            return myInputStream;
        }

        void clear()
        {
        }

        std::ostream& getMockContents()
        {
            return myInputStream;
        }

    private:

        std::stringstream myInputStream;
};

class MockOutputBuffer : public OutputBuffer
{
    public:

        MockOutputBuffer()
        {
        }

        ~MockOutputBuffer()
        {
        }

        bool write()
        {
            return false;
        }

        bool isPacketComplete() const
        {
            return false;
        }

        std::ostream& getOutputStream()
        {
            return myOutputStream;
        }

        void clear()
        {
        }

        std::istream& getMockContents()
        {
            return myOutputStream;
        }

    private:

        std::stringstream myOutputStream;
};

class MockInputOutputBuffer : public InputBuffer, public OutputBuffer
{
    public:

        MockInputOutputBuffer() :
            InputBuffer(),
            OutputBuffer()
        {
        }

        ~MockInputOutputBuffer()
        {
        }

        void sendString(
                const char* outputString
                )
        {
            mock().actualCall("sendString").withParameter("outputString", outputString);
        }

        const char* receiveString()
        {
            return mock().actualCall("receiveString").returnStringValue();
        }

        bool read()
        {
            myStream.str(receiveString());
            return false;
        }

        bool write()
        {
            sendString(myStream.str().c_str());
            return false;
        }

        bool isPacketComplete() const
        {
            return true;
        }

        void clear()
        {
        }

        std::istream& getInputStream()
        {
            return myStream;
        }

        std::ostream& getOutputStream()
        {
            return myStream;
        }

    private:

        std::stringstream myStream;
};

TEST_GROUP(RedBot)
{
    MockInputBuffer* myMockInputBuffer;
    MockOutputBuffer* myMockOutputBuffer;
    MockInputOutputBuffer* myMockInputOutputBuffer;

    void setup()
    {
        myMockInputBuffer = new MockInputBuffer();
        myMockOutputBuffer = new MockOutputBuffer();
        myMockInputOutputBuffer = new MockInputOutputBuffer();
    }

    void teardown()
    {
        delete myMockInputBuffer;
        delete myMockOutputBuffer;
        delete myMockInputOutputBuffer;
        mock().clear();
    }
};

class DigitalOutputRobot : public IterativeRobot
{
    private:

        DigitalOutput dOut;

    public:

        DigitalOutputRobot() :
            IterativeRobot(),
            dOut(4)
        {
        }

        void DisabledPeriodic()
        {
            dOut.Set(true);
        }
};

class DigitalInputRobot : public IterativeRobot
{
    private:

        DigitalInput dIn;

        bool myValue;

    public:

        DigitalInputRobot() :
            IterativeRobot(),
            dIn(6),
            myValue(false)
        {
        }

        void DisabledPeriodic()
        {
            myValue = dIn.Get();
        }

        bool getValue() const
        {
            return myValue;
        }
};

#endif /* ifndef TESTREDBOT_H */
