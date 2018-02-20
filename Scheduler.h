#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>

namespace frc
{
  class Subsystem;

  class Scheduler
  {
  public:

    static Scheduler* GetInstance();

    static void DestroyInstance();

    void Run();

    void AddSubsystem(Subsystem*);

  private:

    typedef std::list<Subsystem*> Subsystems;

    static Scheduler* ourInstance;

    Scheduler();

    Subsystems mySubsystems;

    bool myAreSubsystemsInitialized;
  };
}; /* namespace frc */

#endif /* ifndef SCHEDULER_H */
