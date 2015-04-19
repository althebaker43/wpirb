
#include "FieldControlSystem.h"
#include "IterativeRobot.h"
#include <stdlib.h>


FieldControlSystem* FieldControlSystem::ourInstance = NULL;


FieldControlSystem*
FieldControlSystem::GetInstance()
{
    if (ourInstance == NULL)
    {
        ourInstance = new FieldControlSystem();
    }

    return ourInstance;
}

void
FieldControlSystem::DestroyInstance()
{
    if (ourInstance != NULL)
    {
        delete ourInstance;
        ourInstance = NULL;
    }
}

FieldControlSystem::FieldControlSystem() :
    myRobot(NULL),
    myMode(MODE_DISABLED)
{
}

FieldControlSystem::~FieldControlSystem()
{
}

void
FieldControlSystem::Register(
        IterativeRobot* robot
        )
{
    myRobot = robot;
}

void
FieldControlSystem::EnableAutonomous()
{
    myMode = MODE_AUTO;
    myRobot->AutonomousInit();
}

void
FieldControlSystem::EnableTeleop()
{
    myMode = MODE_TELEOP;
    myRobot->TeleopInit();
}

void
FieldControlSystem::EnableTest()
{
    myMode = MODE_TEST;
    myRobot->TestInit();
}

void
FieldControlSystem::Step()
{
    switch (myMode)
    {
        case MODE_DISABLED:
            myRobot->DisabledPeriodic();
            break;

        case MODE_AUTO:
            myRobot->AutonomousPeriodic();
            break;

        case MODE_TELEOP:
            myRobot->TeleopPeriodic();
            break;

        case MODE_TEST:
            myRobot->TestPeriodic();
            break;

        default:
            break;
    };
}

void
FieldControlSystem::Disable()
{
    myMode = MODE_DISABLED;
    myRobot->DisabledInit();
}
