
#include "RedBot.h"
#include "IterativeRobot.h"


RedBot::RedBot(
        IterativeRobot* program,
        const char*     deviceName
        ) :
    myProgram(program)
{
}

RedBot::~RedBot()
{
}

bool
RedBot::isConnected() const
{
    return true;
}

void
RedBot::modeInit(
        FieldControlSystem::Mode mode
        )
{
    switch (mode)
    {
        case FieldControlSystem::MODE_DISABLED:
            myProgram->DisabledInit();
            break;

        case FieldControlSystem::MODE_AUTO:
            myProgram->AutonomousInit();
            break;

        case FieldControlSystem::MODE_TELEOP:
            myProgram->TeleopInit();
            break;

        case FieldControlSystem::MODE_TEST:
            myProgram->TestInit();
            break;

        default:
            break;
    };
}

void
RedBot::modePeriodic(
        FieldControlSystem::Mode mode
        )
{
    switch (mode)
    {
        case FieldControlSystem::MODE_DISABLED:
            myProgram->DisabledPeriodic();
            break;

        case FieldControlSystem::MODE_AUTO:
            myProgram->AutonomousPeriodic();
            break;

        case FieldControlSystem::MODE_TELEOP:
            myProgram->TeleopPeriodic();
            break;

        case FieldControlSystem::MODE_TEST:
            myProgram->TestPeriodic();
            break;

        default:
            break;
    };
}
