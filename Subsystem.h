#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>

namespace frc
{
  class Command;

  class Subsystem
  {
  public:

    Subsystem(const std::string&);

    virtual ~Subsystem(){}

    void ProcessCommands();

    void SetNextCommand(Command*);

  private:

    Subsystem();

    Command* myCurrentCommand;

    Command* myNextCommand;
  };
}; /* namespace frc */

#endif /* ifndef SUBSYSTEM_H */
