#ifndef COMMAND_H
#define COMMAND_H

namespace frc
{
  class Subsystem;

  class Command
  {
  public:

    friend class Subsystem;

    Command();

    virtual ~Command(){}

    void Requires(Subsystem*);

    void Start();

    bool IsInterruptible() const;

    void SetInterruptible(bool isInterruptible);

  protected:

    virtual void Initialize();

    virtual void Execute();

    virtual bool IsFinished() = 0;

    virtual void End();

    virtual void Interrupted();

  private:

    Subsystem* mySubsystem;

    bool myIsInterruptible;
  };
}; /* namespace frc */

#endif /* ifndef COMMAND_H */
