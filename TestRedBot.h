#ifndef TESTREDBOT_H
#define TESTREDBOT_H

#include "IterativeRobot.h"
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "IOBuffer.h"
#include "CppUTest/TestHarness.h"
#include <stdio.h>

TEST_GROUP(RedBot)
{
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

        std::istream& getContents()
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

        std::ostream& getContents()
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

#endif /* ifndef TESTREDBOT_H */
