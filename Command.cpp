
#include "Command.h"
#include "Subsystem.h"
#include <stdlib.h>

using namespace frc;


Command::Command() :
  mySubsystem(DefaultSubsystem::GetInstance()),
  myIsInterruptible(true),
  myIsEndCalled(false)
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
Command::SetEndCalled(bool isEndCalled)
{
  myIsEndCalled = isEndCalled;
}

bool
Command::IsEndCalled() const
{
  return myIsEndCalled;
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
