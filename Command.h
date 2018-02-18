#ifndef COMMAND_H
#define COMMAND_H

namespace frc
{
  class Subsystem;

  class Command
  {
  public:

    friend class Subsystem;
    friend class DefaultSubsystem;

    Command();

    virtual ~Command(){}

    void Requires(Subsystem*);

    virtual void Start();

    bool IsInterruptible() const;

    void SetInterruptible(bool isInterruptible);

    bool IsEndCalled() const;

  protected:

    virtual void Initialize();

    virtual void Execute();

    virtual bool IsFinished() = 0;

    virtual void End();

    virtual void Interrupted();

    Subsystem* mySubsystem;

  private:

    void SetEndCalled(bool isEndCalled);

    bool myIsInterruptible;

    bool myIsEndCalled;
  };
}; /* namespace frc */

#endif /* ifndef COMMAND_H */
