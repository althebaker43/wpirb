
#include "TestRedBot.h"
#include "RedBot.h"


TEST(RedBot, CommandTest)
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
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    robot.modePeriodic(mode);

    mock().checkExpectations();
}

TEST(RedBot, ResponseTest)
{
    DigitalInputRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x01\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    robot.modePeriodic(mode);
    robot.modePeriodic(mode);

    CHECK_TRUE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());
    mock().checkExpectations();
}
