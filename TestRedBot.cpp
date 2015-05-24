
#include "TestRedBot.h"
#include "Packet.h"
#include "RedBot.h"


void
Exchange(
        const std::vector<Packet*>& requestPackets,
        const std::vector<Packet*>& responsePackets,
        std::list<std::string>&     packetStrings,
        RedBot&                     robot,
        FieldControlSystem::Mode    mode,
        size_t                      numCycles
        )
{
    for(
            std::vector<Packet*>::const_iterator packetIter = requestPackets.begin();
            packetIter != requestPackets.end();
            ++packetIter
       )
    {
        std::ostringstream packetStream;
        Packet* packet = *packetIter;

        packetStream << *packet;
        packetStrings.push_back(packetStream.str());

        mock().expectOneCall("sendString").withParameter("outputString", packetStrings.back().c_str());
    }

    for(
            std::vector<Packet*>::const_iterator packetIter = responsePackets.begin();
            packetIter != responsePackets.end();
            ++packetIter
       )
    {
        std::ostringstream packetStream;
        Packet* packet = *packetIter;

        packetStream << *packet;
        packetStrings.push_back(packetStream.str());

        mock().expectOneCall("receiveString").andReturnValue(packetStrings.back().c_str());
    }

    robot.modeInit(mode);

    for(
            size_t cycleIdx = 0;
            cycleIdx < numCycles;
            ++cycleIdx
       )
    {
        robot.modePeriodic(mode);
    }
}

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

TEST(RedBot, DriveTest)
{
    DriveRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_TELEOP;

    myRequestPackets.resize(3);
    myRequestPackets[0] = new MotorDrivePacket(
            MotorDrivePacket::MOTOR_LEFT,
            255,
            MotorDrivePacket::DIR_FORWARD
            );
    myRequestPackets[1] = new MotorDrivePacket(
            MotorDrivePacket::MOTOR_RIGHT,
            255,
            MotorDrivePacket::DIR_FORWARD
            );
    myRequestPackets[2] = new PingPacket();

    myResponsePackets.resize(3);
    myResponsePackets[0] = new AcknowledgePacket();
    myResponsePackets[1] = new AcknowledgePacket();
    myResponsePackets[2] = new AcknowledgePacket();

    Exchange(
            myRequestPackets,
            myResponsePackets,
            myPacketStrings,
            robot,
            mode,
            1
            );

    mock().checkExpectations();
}
