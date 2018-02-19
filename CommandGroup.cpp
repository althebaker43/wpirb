
#include "CommandGroup.h"
#include "Subsystem.h"
#include <stdlib.h>

using namespace frc;


CommandGroup::CommandGroup() :
  Command()
{
}

CommandGroup::~CommandGroup()
{
}

void
CommandGroup::AddSequential(Command* command)
{
  mySteps.push_back(Step(1, command));
}

void
CommandGroup::AddParallel(Command* command)
{
  if (!(mySteps.empty()))
    {
      mySteps.back().push_back(command);
    }
}

void
CommandGroup::Start()
{
  if (mySteps.empty())
    {
      return;
    }

  for (Step::const_iterator cmdIterator = mySteps.front().begin();
       cmdIterator != mySteps.front().end();
       ++cmdIterator)
    {
      Command* cmd = *cmdIterator;

      cmd->Start();
    }

  myStepIterator = mySteps.begin();

  mySubsystem->SetNextCommand(this);
}

void
CommandGroup::Execute()
{
  for (Step::const_iterator cmdIterator = myStepIterator->begin();
       cmdIterator != myStepIterator->end();
       ++cmdIterator)
    {
      Command* cmd = *cmdIterator;

      if (!(cmd->IsEndCalled()))
	{
	  return;
	}
    }

  if (++myStepIterator == mySteps.end())
    {
      return;
    }

  for (Step::const_iterator cmdIterator = myStepIterator->begin();
       cmdIterator != myStepIterator->end();
       ++cmdIterator)
    {
      Command* cmd = *cmdIterator;

      cmd->Start();
    }
}

bool
CommandGroup::IsFinished()
{
  return (myStepIterator == mySteps.end());
}
