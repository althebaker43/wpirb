#ifndef TESTREDBOT_H
#define TESTREDBOT_H

#include "IterativeRobot.h"
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "IOBuffer.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <stdio.h>

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

        bool readPacket()
        {
            myStream.str(receiveString());
            return false;
        }

        bool writePacket()
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
            myStream.clear();
            myStream.str("");
        }

        std::istream& getInputStream()
        {
            return myStream;
        }

        std::ostream& getOutputStream()
        {
            clear();
            return myStream;
        }

        void getContents(
                std::string& contents
                )
        {
            contents = myStream.str();
        }

    private:

        std::stringstream myStream;
};

TEST_GROUP(RedBot)
{
    MockInputOutputBuffer* myMockInputOutputBuffer;

    void setup()
    {
        myMockInputOutputBuffer = new MockInputOutputBuffer();
    }

    void teardown()
    {
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
