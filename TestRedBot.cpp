
#include "IterativeRobot.h"
#include "RedBot.h"
#include "DigitalOutput.h"
#include "CppUTest/TestHarness.h"


TEST_GROUP(RedBot)
{
    FILE* myPacketFile;

    void setup()
    {
        myPacketFile = tmpfile();
    }
};

class DigitalIORobot : public IterativeRobot
{
    private:

        DigitalOutput dOut;

    public:

        DigitalIORobot() :
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
    DigitalIORobot program;
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
