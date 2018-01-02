#ifndef SCHEDULER_H
#define SCHEDULER_H

namespace frc
{
  class Scheduler
  {
  public:

    static Scheduler* GetInstance();

    static void DestroyInstance();

    void Run();

  private:

    static Scheduler* ourInstance;
  };
}; /* namespace frc */

#endif /* ifndef SCHEDULER_H */
