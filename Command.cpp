
#include "Command.h"
#include "Subsystem.h"
#include <stdlib.h>

using namespace frc;


Command::Command() :
  mySubsystem(NULL)
{
}

void
Command::Requires(Subsystem* subsystem)
{
  mySubsystem = subsystem;
}

void
Command::Start()
{
  mySubsystem->SetNextCommand(this);
}

void
Command::Initialize()
{
}
