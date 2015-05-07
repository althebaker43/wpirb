
#include "TestRedBot.h"
#include "RedBot.h"


TEST(RedBot, CommandTest)
{
    DigitalOutputRobot program;
    MockInputBuffer* inputBuffer = new MockInputBuffer();
    MockOutputBuffer* outputBuffer = new MockOutputBuffer();
    RedBot robot(
            &program,
            inputBuffer,
            outputBuffer
            );
    std::string mockOutput;

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;

    robot.modeInit(mode);
    robot.modePeriodic(mode);

    outputBuffer->getMockContents() >> mockOutput;

    STRCMP_EQUAL("\xFF\x02\x04\x02\xFF", mockOutput.c_str());

    robot.modePeriodic(mode);

    outputBuffer->getMockContents() >> mockOutput;

    STRCMP_EQUAL("\xFF\x02\x04\x02\xFF", mockOutput.c_str());
}

TEST(RedBot, ResponseTest)
{
    DigitalInputRobot program;
    MockInputBuffer* inputBuffer = new MockInputBuffer();
    MockOutputBuffer* outputBuffer = new MockOutputBuffer();
    RedBot robot(
            &program,
            inputBuffer,
            outputBuffer
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;

    inputBuffer->getMockContents() << "\xFF\x81\x06\x02\xFF";    // First response packet

    robot.modeInit(mode);
    robot.modePeriodic(mode);

    inputBuffer->getMockContents() << "\xFF\x81\x06\x01\xFF";    // Second response packet

    robot.modePeriodic(mode);

    CHECK_TRUE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());
}
