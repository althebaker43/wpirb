
#include "IterativeRobot.h"
#include "RedBot.h"
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "CppUTest/TestHarness.h"


TEST_GROUP(RedBot)
{
    FILE* myPacketFile;

    void setup()
    {
        myPacketFile = tmpfile();
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

TEST(RedBot, CommandTest)
{
    DigitalOutputRobot program;
    RedBot robot(
            &program,
            myPacketFile
            );
    char packetBytes [6];

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;

    robot.modeInit(mode);
    robot.modePeriodic(mode);

    fflush(myPacketFile);
    rewind(myPacketFile);

    CHECK(fgets(packetBytes, 6, myPacketFile) != NULL);
    STRCMP_EQUAL("\xFF\x02\x04\x02\xFF", packetBytes);

    rewind(myPacketFile);
    fputs("\xFF\xFF\xFF\xFF\xFF\xFF", myPacketFile);
    fflush(myPacketFile);
    rewind(myPacketFile);

    robot.modePeriodic(mode);

    fflush(myPacketFile);
    rewind(myPacketFile);

    CHECK(fgets(packetBytes, 6, myPacketFile) != NULL);
    STRCMP_EQUAL("\xFF\x02\x04\x02\xFF", packetBytes);
}

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

TEST(RedBot, ResponseTest)
{
    DigitalInputRobot program;
    RedBot robot(
            &program,
            myPacketFile
            );
    //char packetBytes [6];

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;

    fputs("\xFF\xFF\xFF\xFF", myPacketFile);        // Space for first outgoing packet
    fputs("\xFF\x81\x06\x02\xFF", myPacketFile);    // First response packet
    fputs("\xFF\xFF\xFF\xFF", myPacketFile);        // Space for second outgoing packet
    fputs("\xFF\x81\x06\x01\xFF", myPacketFile);    // Second response packet
    fflush(myPacketFile);
    rewind(myPacketFile);

    robot.modeInit(mode);
    robot.modePeriodic(mode);
    robot.modePeriodic(mode);

    CHECK_TRUE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());
}
