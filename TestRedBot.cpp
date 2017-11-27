
#define CPPUTEST_MEM_LEAK_DETECTION_DISABLED 1

#include "TestRedBot.h"
#include "RedBot.h"
#include "SmartDashboard.h"
#include "networktables/NetworkTableInstance.h"


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
            myMockInputOutputBuffer,
            new RedBotPacketGenerator()
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    // cycle 1: configure
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x05\x04\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x84\x04\x01\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // cycle 2: command
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x02\x04\x02\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    robot.modePeriodic(mode);
    robot.modePeriodic(mode);

    mock().checkExpectations();
}

TEST(RedBot, ResponseTest)
{
    DigitalInputRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer,
            new RedBotPacketGenerator()
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    // Pin configuration
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x05\x06\x02\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x84\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x01\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x01\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Delayed pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    // Get pin value
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x03\x06\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x81\x06\x02\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());

    robot.modePeriodic(mode);

    CHECK_FALSE(program.getValue());

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
            myMockInputOutputBuffer,
            new RedBotPacketGenerator()
            );

    myRequestPackets.push_back(new PingPacket());                                   // Cycle 1
    myRequestPackets.push_back(new PinConfigPacket(6, PinConfigPacket::DIR_INPUT));
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 2
    myRequestPackets.push_back(new DigitalInputPacket(6));
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 3
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 4
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 5
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 6
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 7
    myRequestPackets.push_back(new PingPacket());
    myRequestPackets.push_back(new PingPacket());                                     // Cycle 8
    myRequestPackets.push_back(new DigitalInputPacket(6));
    myRequestPackets.push_back(new PingPacket());

    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 1
    myResponsePackets.push_back(new PinConfigInfoPacket(6, PinConfigInfoPacket::DIR_INPUT));
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 2
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 3
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 4
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 5
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 6
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 7
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket()); // Cycle 8
    myResponsePackets.push_back(new AcknowledgePacket());
    myResponsePackets.push_back(new AcknowledgePacket());

    std::list<std::string> packetStrings;

    Exchange(
            myRequestPackets,
            myResponsePackets,
            packetStrings,
            robot,
            FieldControlSystem::MODE_DISABLED,
            8
            );

    mock().checkExpectations();
}

TEST(RedBot, DriveTest)
{
    DriveRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer,
            new RedBotPacketGenerator()
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_TELEOP;

    myRequestPackets.resize(4);
    myRequestPackets[0] = new PingPacket();
    myRequestPackets[1] = new MotorDrivePacket(
            MotorDrivePacket::MOTOR_LEFT,
            255,
            MotorDrivePacket::DIR_FORWARD
            );
    myRequestPackets[2] = new MotorDrivePacket(
            MotorDrivePacket::MOTOR_RIGHT,
            255,
            MotorDrivePacket::DIR_FORWARD
            );
    myRequestPackets[3] = new PingPacket();

    myResponsePackets.resize(4);
    myResponsePackets[0] = new AcknowledgePacket();
    myResponsePackets[1] = new AcknowledgePacket();
    myResponsePackets[2] = new AcknowledgePacket();
    myResponsePackets[3] = new AcknowledgePacket();

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
    frc::IterativeRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer,
            new RedBotPacketGenerator()
            );
    std::list<std::string> sentStrings;
    std::list<std::string> receivedStrings;

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
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
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
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
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
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
    BPACKET_EQUAL("\xFF\x81\x0A\xFF", receivedStrings.front().c_str());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
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
    frc::IterativeRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer,
            new RedBotPacketGenerator()
            );

    FieldControlSystem::Mode mode = FieldControlSystem::MODE_DISABLED;
    robot.modeInit(mode);

    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());

    for (int i = 0; i < 5; i++)
    {
        mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
        mock().expectOneCall("receiveString").andReturnValue("");
    }
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_UNRESPONSIVE, robot.getStatus());

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\x01\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");
    robot.modePeriodic(mode);

    mock().checkExpectations();
    CHECK_EQUAL(RedBot::STATUS_GOOD, robot.getStatus());
}

TEST(RedBot, ResyncTest)
{
    frc::IterativeRobot program;
    RedBot robot(
            &program,
            myMockInputOutputBuffer,
            myMockInputOutputBuffer,
            new RedBotPacketGenerator()
            );

    mock().expectOneCall("sendString").withParameter("outputString", "\xFF\xFF\xFF\xFF\xFF");
    mock().expectOneCall("receiveString").andReturnValue("\xFF\x82\xFF");

    robot.resync();

    mock().checkExpectations();
}

TEST(Timer, BasicTest)
{
    MockTimer timer(MockTimeAccessor);
    MockTime.tv_sec = 0;
    MockTime.tv_nsec = 0;

    CHECK_EQUAL(0.0, timer.Get());

    timer.Start();
    MockTime.tv_sec = 1;

    CHECK_EQUAL(1.0, timer.Get());

    timer.Stop();
    MockTime.tv_sec = 2;

    CHECK_EQUAL(1.0, timer.Get());

    timer.Start();

    CHECK_EQUAL(1.0, timer.Get());
    CHECK_FALSE(timer.HasPeriodPassed(1.25));

    MockTime.tv_nsec = 5e8;

    CHECK_EQUAL(1.5, timer.Get());
    CHECK_TRUE(timer.HasPeriodPassed(1.25));

    timer.Reset();

    CHECK_EQUAL(0.0, timer.Get());

    timer.Stop();

    CHECK_EQUAL(0.0, timer.Get());
}

TEST_GROUP(SmartDashboard)
{
  void setup()
  {
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
  }

  void teardown()
  {
    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
  }
};

TEST(SmartDashboard, BooleanTest)
{
  std::shared_ptr<nt::NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard");
  nt::NetworkTableEntry entry = table->GetEntry("testVar");

  CHECK_FALSE(entry.Exists());
  CHECK_TRUE(frc::SmartDashboard::GetBoolean("testVar", true));
  CHECK_FALSE(frc::SmartDashboard::GetBoolean("testVar", false));

  CHECK_TRUE(frc::SmartDashboard::PutBoolean("testVar", true));

  entry = table->GetEntry("testVar");

  CHECK_TRUE(entry.Exists());
  CHECK_TRUE(table->GetBoolean("testVar", false));
  CHECK_TRUE(frc::SmartDashboard::GetBoolean("testVar", false));

  CHECK_TRUE(frc::SmartDashboard::PutBoolean("testVar", false));

  entry = table->GetEntry("testVar");

  CHECK_TRUE(entry.Exists());
  CHECK_FALSE(table->GetBoolean("testVar", true));
  CHECK_FALSE(frc::SmartDashboard::GetBoolean("testVar", true));
}
