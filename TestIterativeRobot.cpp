
#include "IterativeRobot.h"
#include "CppUTest/TestHarness.h"


TEST_GROUP(IterativeRobot)
{
};

class CountRobot : public IterativeRobot
{
    public:

        CountRobot()
        {
        }
};

TEST(IterativeRobot, BasicTest)
{
    CountRobot robot;

    robot.RobotInit();
}
