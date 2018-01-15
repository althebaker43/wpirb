
#include "Subsystem.h"
#include "Scheduler.h"
#include "Command.h"

using namespace frc;


Subsystem::Subsystem(const std::string& name) :
  myCurrentCommand(NULL),
  myNextCommand(NULL)
{
  Scheduler::GetInstance()->AddSubsystem(this);
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
	}
      else
	{
	  myCurrentCommand = myNextCommand;
	  myNextCommand->Initialize();
	}
      myNextCommand = NULL;
    }
}
