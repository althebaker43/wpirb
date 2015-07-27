
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

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x04\x04\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
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

    // Pin configuration and get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x04\x06\x02\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x01\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Delayed pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    //mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x01\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    //mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_TRUE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_TRUE(program.getValue());

    mock().checkExpectations();
}

TEST(RedBot, ResponseTimeoutTest)
{
    DigitalInputRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer
            );

    myRequestPackets.push_back(new PinConfigPacket(6, PinConfigPacket::DIR_INPUT));   // Cycle 1
    myRequestPackets.push_back(new DigitalInputPacket(6));
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 2
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 3
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 4
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 5
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 6
    myRequestPackets.push_back(new DigitalInputPacket(6));                            // Cycle 7
    myRequestPackets.push_back(new PingPacket());

    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 1
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 2
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 3
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 4
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 5
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 6
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 7
    myResponsePackets.push_back(new AcknowledgePacket());

    std::list<std::string> packetStrings;

    Exchange(
            myRequestPackets,
            myResponsePackets,
            packetStrings,
            robot,
            FieldControlSystem::MODE_DISABLED,
            7
            );

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

TEST(RedBot, UnrecognizedPacketTest)
{
    IterativeRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer
            );
    std::list<std::string> sentStrings;
    std::list<std::string> receivedStrings;

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());

    robot.getLastBinaryTransaction(
            sentStrings,
            receivedStrings
            );

    STRCMP_EQUAL("\xFF\x01\xFF", sentStrings.front().c_str());
    STRCMP_EQUAL("\xFF\x82\xFF", receivedStrings.front().c_str());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x8F\xFF");
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_INCOHERENT, robot.getStatus());

    robot.getLastBinaryTransaction(
            sentStrings,
            receivedStrings
            );

    STRCMP_EQUAL("\xFF\x01\xFF", sentStrings.front().c_str());
    STRCMP_EQUAL("\xFF\x8F", receivedStrings.front().c_str());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x0A\xFF");
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_INCOHERENT, robot.getStatus());

    robot.getLastBinaryTransaction(
            sentStrings,
            receivedStrings
            );

    STRCMP_EQUAL("\xFF\x01\xFF", sentStrings.front().c_str());
    STRCMP_EQUAL("\xFF\x81\x0A\xFF", receivedStrings.front().c_str());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());

    robot.getLastBinaryTransaction(
            sentStrings,
            receivedStrings
            );

    STRCMP_EQUAL("\xFF\x01\xFF", sentStrings.front().c_str());
    STRCMP_EQUAL("\xFF\x82\xFF", receivedStrings.front().c_str());
}

TEST(RedBot, UnresponsiveTest)
{
    IterativeRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("");
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_UNRESPONSIVE, robot.getStatus());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());
}

TEST(RedBot, ResyncTest)
{
    IterativeRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer
            );

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\xFF\xFF\xFF\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    robot.resync();

    mock().checkExpectations();
}
