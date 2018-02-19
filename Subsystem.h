#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>
#include <set>

namespace frc
{
  class Command;

  class Subsystem
  {
  public:

    Subsystem(const std::string&);

    virtual ~Subsystem(){}

    void SetDefaultCommand(Command* command);

    virtual void ProcessCommands();

    virtual void SetNextCommand(Command*);

  protected:

    Subsystem();

  private:

    Command* myDefaultCommand;

    Command* myCurrentCommand;

    Command* myNextCommand;
  };

  class DefaultSubsystem : public Subsystem
  {
  public:

    static DefaultSubsystem* GetInstance();

    static void DestroyInstance();

  private:

    typedef std::set<Command*> Commands;

    DefaultSubsystem();

    ~DefaultSubsystem();

    void ProcessCommands();

    void SetNextCommand(Command*);

    static DefaultSubsystem* ourInstance;

    Commands myNextCommands;

    Commands myCurrentCommands;
  };
}; /* namespace frc */

#endif /* ifndef SUBSYSTEM_H */
