
#include "FieldControlSystem.h"
#include "RedBot.h"
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
        RedBot* robot
        )
{
    myRobot = robot;
}

void
FieldControlSystem::EnableAutonomous()
{
    myMode = MODE_AUTO;
    myRobot->modeInit(MODE_AUTO);
}

void
FieldControlSystem::EnableTeleop()
{
    myMode = MODE_TELEOP;
    myRobot->modeInit(MODE_TELEOP);
}

void
FieldControlSystem::EnableTest()
{
    myMode = MODE_TEST;
    myRobot->modeInit(MODE_TEST);
}

void
FieldControlSystem::Step()
{
    myRobot->modePeriodic(myMode);
}

void
FieldControlSystem::Disable()
{
    myMode = MODE_DISABLED;
    myRobot->modeInit(MODE_DISABLED);
}
