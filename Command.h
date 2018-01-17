#ifndef COMMAND_H
#define COMMAND_H

namespace frc
{
  class Subsystem;

  class Command
  {
  public:

    Command();

    virtual ~Command(){}

    void Requires(Subsystem*);

    void Start();

    virtual void Initialize();

    virtual void Execute();

    virtual bool IsFinished() = 0;

    virtual void End();

  private:

    Subsystem* mySubsystem;
  };
}; /* namespace frc */

#endif /* ifndef COMMAND_H */
