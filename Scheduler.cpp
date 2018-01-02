
#include "Scheduler.h"
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
}

void
Scheduler::Run()
{
}
