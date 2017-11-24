
#include "IterativeRobot.h"
#include "RedBot.h"
#include "RedBotPacket.h"
#include "FieldControlSystem.h"
#include "CppUTest/TestHarness.h"


TEST_GROUP(IterativeRobot)
{
    void teardown()
    {
        FieldControlSystem::DestroyInstance();
    }
};

class CountRobot : public frc::IterativeRobot
{
    private:

        size_t myCounts[FieldControlSystem::NUM_MODES];

    public:

        CountRobot()
        {
            for(
                    size_t countIdx = 0;
                    countIdx < FieldControlSystem::NUM_MODES;
                    ++countIdx
               )
            {
                myCounts[countIdx] = 0;
            }
        }

        size_t getCount(FieldControlSystem::Mode mode) const
        {
            return myCounts[mode];
        }

        void DisabledPeriodic()
        {
            ++myCounts[FieldControlSystem::MODE_DISABLED];
        }

        void AutonomousInit()
        {
            myCounts[FieldControlSystem::MODE_AUTO] = 0;
        }

        void AutonomousPeriodic()
        {
            ++myCounts[FieldControlSystem::MODE_AUTO];
        }
};

class MockRedBot : public RedBot
{
    public:

        MockRedBot(
		frc::IterativeRobot* program
                ) :
            RedBot(
                    program,
                    (const char*)NULL, // Do not attempt to establish serial link
                    new RedBotPacketGenerator()
                  )
        {
        }

        bool isConnected() const
        {
            return true;
        }
};

TEST(IterativeRobot, BasicTest)
{
    CountRobot robot;
    robot.RobotInit();
    robot.DisabledInit();

    CHECK_EQUAL(0, robot.getCount(FieldControlSystem::MODE_DISABLED));

    robot.DisabledPeriodic();

    CHECK_EQUAL(1, robot.getCount(FieldControlSystem::MODE_DISABLED));

    robot.DisabledPeriodic();

    CHECK_EQUAL(2, robot.getCount(FieldControlSystem::MODE_DISABLED));
}

TEST(IterativeRobot, BasicFCSTest)
{
    CountRobot robot;
    MockRedBot redBot(&robot);
    FieldControlSystem* fcs = FieldControlSystem::GetInstance();
    fcs->Register(&redBot);
    fcs->EnableAutonomous();

    CHECK_EQUAL(0, robot.getCount(FieldControlSystem::MODE_AUTO));

    fcs->Step();

    CHECK_EQUAL(1, robot.getCount(FieldControlSystem::MODE_AUTO));

    fcs->Step();

    CHECK_EQUAL(2, robot.getCount(FieldControlSystem::MODE_AUTO));

    fcs->Disable();

    CHECK_EQUAL(0, robot.getCount(FieldControlSystem::MODE_DISABLED));

    fcs->Step();

    CHECK_EQUAL(1, robot.getCount(FieldControlSystem::MODE_DISABLED));
    CHECK_EQUAL(2, robot.getCount(FieldControlSystem::MODE_AUTO));

    fcs->EnableAutonomous();

    CHECK_EQUAL(0, robot.getCount(FieldControlSystem::MODE_AUTO));

    fcs->Step();

    CHECK_EQUAL(1, robot.getCount(FieldControlSystem::MODE_AUTO));
    CHECK_EQUAL(1, robot.getCount(FieldControlSystem::MODE_DISABLED));
}
