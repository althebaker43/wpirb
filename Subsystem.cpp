
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
      myCurrentCommand->SetEndCalled(false);

      myCurrentCommand->Execute();

      if (myCurrentCommand->IsFinished())
	{
	  myCurrentCommand->End();
	  myCurrentCommand->SetEndCalled(true);
	  myCurrentCommand = NULL;
	}
    }
}


DefaultSubsystem* DefaultSubsystem::ourInstance = NULL;

DefaultSubsystem*
DefaultSubsystem::GetInstance()
{
  if (!ourInstance)
    {
      ourInstance = new DefaultSubsystem;
    }

  return ourInstance;
}

void
DefaultSubsystem::DestroyInstance()
{
  delete ourInstance;
  ourInstance = NULL;
}

DefaultSubsystem::DefaultSubsystem() :
  Subsystem("_default")
{
}

DefaultSubsystem::~DefaultSubsystem()
{
}

void
DefaultSubsystem::SetNextCommand(Command* command)
{
  myNextCommands.push_back(command);
}

void
DefaultSubsystem::ProcessCommands()
{
  for (Commands::const_iterator nextCmdIter = myNextCommands.begin(); nextCmdIter != myNextCommands.end(); ++nextCmdIter)
    {
      Command* cmd = *nextCmdIter;
      cmd->Initialize();
      myCurrentCommands.push_back(cmd);
    }
  myNextCommands.clear();

  Commands::iterator cmdIter = myCurrentCommands.begin();

  while (cmdIter != myCurrentCommands.end())
    {
      Command* cmd = *cmdIter;
      cmd->SetEndCalled(false);
      cmd->Execute();
      if (cmd->IsFinished())
	{
	  cmd->End();
	  cmd->SetEndCalled(true);
	  cmdIter = myCurrentCommands.erase(cmdIter);
	}
      else
	{
	  ++cmdIter;
	}
    }
}
