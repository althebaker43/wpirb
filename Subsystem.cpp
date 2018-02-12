
#include "Subsystem.h"
#include "Scheduler.h"
#include "Command.h"

using namespace frc;


Subsystem::Subsystem(const std::string& name) :
  myDefaultCommand(NULL),
  myCurrentCommand(NULL),
  myNextCommand(NULL)
{
  Scheduler::GetInstance()->AddSubsystem(this);
}

void
Subsystem::SetDefaultCommand(Command* command)
{
  myDefaultCommand = command;
}

void
Subsystem::SetNextCommand(Command* command)
{
  myNextCommand = command;
}

void
Subsystem::ProcessCommands()
{
  if (myNextCommand)
    {
      if (myCurrentCommand)
	{
	  if (myCurrentCommand->IsInterruptible())
	    {
	      myCurrentCommand->Interrupted();
	      myCurrentCommand = myNextCommand;
	      myCurrentCommand->Initialize();
	    }
	}
      else
	{
	  myCurrentCommand = myNextCommand;
	  myCurrentCommand->Initialize();
	}
      myNextCommand = NULL;
    }

  if (!myCurrentCommand && myDefaultCommand)
    {
      myCurrentCommand = myDefaultCommand;
      myCurrentCommand->Initialize();
    }

  if (myCurrentCommand)
    {
      myCurrentCommand->Execute();

      if (myCurrentCommand->IsFinished())
	{
	  myCurrentCommand->End();
	  myCurrentCommand = NULL;
	}
    }
}
