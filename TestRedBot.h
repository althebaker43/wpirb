#ifndef TESTREDBOT_H
#define TESTREDBOT_H

#include "IterativeRobot.h"
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "RobotDrive.h"
#include "Timer.h"
#include "IOBuffer.h"
#include "FieldControlSystem.h"
#include "TestUtils.h"
#include "CppUTestExt/MockSupport.h"
#include <stdio.h>
#include <vector>
#include <list>

class RedBot;

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

        void resync()
        {
            sendString("\xFF\xFF\xFF\xFF\xFF");
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

    std::vector<Packet*> myRequestPackets;

    std::vector<Packet*> myResponsePackets;

    std::list<std::string> myPacketStrings;

    void setup()
    {
        myMockInputOutputBuffer = new MockInputOutputBuffer();
    }

    void teardown()
    {
        for(
                std::vector<Packet*>::const_iterator packetIter = myRequestPackets.begin();
                packetIter != myRequestPackets.end();
                ++packetIter
           )
        {
            delete (*packetIter);
        }
        myRequestPackets.clear();

        for(
                std::vector<Packet*>::const_iterator packetIter = myResponsePackets.begin();
                packetIter != myResponsePackets.end();
                ++packetIter
           )
        {
            delete (*packetIter);
        }
        myResponsePackets.clear();

        myPacketStrings.clear();

        delete myMockInputOutputBuffer;
        mock().clear();
    }
};

void
Exchange(
        const std::vector<Packet*>& requestPackets,
        const std::vector<Packet*>& responsePackets,
        RedBot&                     robot,
        FieldControlSystem::Mode    mode,
        size_t                      numCycles
        );

class DigitalOutputRobot : public frc::IterativeRobot
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

class DigitalInputRobot : public frc::IterativeRobot
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

class DriveRobot : public frc::IterativeRobot
{
    private:

        RobotDrive drive;

    public:

        DriveRobot() :
            IterativeRobot(),
            drive(0,0)
        {
        }

        void TeleopInit()
        {
            drive.Drive(1.0, 0);
        }

        void TeleopPeriodic()
        {
        }
};

TEST_GROUP(Timer)
{
};

struct timespec MockTime;

int
MockTimeAccessor(
        clockid_t           clock,
        struct timespec*    time
        )
{
    time->tv_sec = MockTime.tv_sec;
    time->tv_nsec = MockTime.tv_nsec;
    return 0;
}

class MockTimer : public frc::Timer
{
    public:

        MockTimer(
                TimeAccessor accessor
                ) :
            Timer()
        {
            Timer::getTime = accessor;
        }
};

#endif /* ifndef TESTREDBOT_H */
