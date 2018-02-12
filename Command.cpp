
#include "Command.h"
#include "Subsystem.h"
#include <stdlib.h>

using namespace frc;


Command::Command() :
  mySubsystem(NULL),
  myIsInterruptible(true)
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

bool
Command::IsInterruptible() const
{
  return myIsInterruptible;
}

void
Command::SetInterruptible(bool isInterruptible)
{
  myIsInterruptible = isInterruptible;
}

void
Command::Initialize()
{
}

void
Command::Execute()
{
}

void
Command::End()
{
}

void
Command::Interrupted()
{
}
