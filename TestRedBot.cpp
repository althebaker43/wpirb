
#include "TestRedBot.h"
#include "RedBot.h"


TEST(RedBot, CommandTest)
{
    DigitalOutputRobot program;
    RedBot robot(
            &program,
            myMockInputBuffer,
            myMockOutputBuffer
            );
    std::string mockOutput;

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;

    robot.modeInit(mode);
    robot.modePeriodic(mode);

    myMockOutputBuffer->getMockContents() >> mockOutput;

    STRCMP_EQUAL("\xFF\x02\x04\x02\xFF", mockOutput.c_str());

    robot.modePeriodic(mode);

    myMockOutputBuffer->getMockContents() >> mockOutput;

    STRCMP_EQUAL("\xFF\x02\x04\x02\xFF", mockOutput.c_str());
}

TEST(RedBot, ExchangeTest)
{
    DigitalOutputRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x02\x04\x02\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    robot.modePeriodic(mode);

    mock().checkExpectations();
}

TEST(RedBot, ResponseTest)
{
    DigitalInputRobot program;
    RedBot robot(
            &program,
            myMockInputBuffer,
            myMockOutputBuffer
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;

    myMockInputBuffer->getMockContents() << "\xFF\x81\x06\x02\xFF";    // First response packet
    myMockInputBuffer->getMockContents() << "\xFF\x81\x06\x01\xFF";    // Second response packet

    robot.modeInit(mode);
    robot.modePeriodic(mode);
    robot.modePeriodic(mode);

    CHECK_TRUE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());
}
