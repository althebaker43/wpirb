
#include "Scheduler.h"
#include "Subsystem.h"
#include <stdlib.h>

using namespace frc;


Scheduler* Scheduler::ourInstance = NULL;


Scheduler*
Scheduler::GetInstance()
{
  if (ourInstance == NULL)
    {
      ourInstance = new Scheduler;
    }

  return ourInstance;
}

void
Scheduler::DestroyInstance()
{
  delete ourInstance;
  ourInstance = NULL;

  DefaultSubsystem::DestroyInstance();
}

Scheduler::Scheduler() :
  myAreSubsystemsInitialized(false)
{
}

void
Scheduler::Run()
{
  if (!myAreSubsystemsInitialized)
    {
      for (Subsystems::const_iterator sysIter = mySubsystems.begin(); sysIter != mySubsystems.end(); ++sysIter)
	{
	  Subsystem* subsystem = *sysIter;
	  subsystem->InitDefaultCommand();
	}
      myAreSubsystemsInitialized = true;
    }
      
  for (Subsystems::const_iterator sysIter = mySubsystems.begin(); sysIter != mySubsystems.end(); ++sysIter)
    {
      Subsystem* subsystem = *sysIter;
      subsystem->ProcessCommands();
    }
}

void
Scheduler::AddSubsystem(Subsystem* subsystem)
{
  mySubsystems.push_back(subsystem);
}
