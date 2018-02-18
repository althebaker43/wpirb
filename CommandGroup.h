#ifndef COMMANDGROUP_H
#define COMMANDGROUP_H

#include "Command.h"
#include <list>

namespace frc
{
  class CommandGroup : public Command
  {
  public:

    CommandGroup();

    virtual ~CommandGroup();

    void AddSequential(Command* command);

    void Start();

  protected:

    void Execute();

    bool IsFinished();

  private:

    using Step = std::list<Command*>;
    using Steps = std::list<Step>;

    Steps mySteps;

    Steps::const_iterator myStepIterator;
  };
};

#endif /* ifndef COMMANDGROUP_H */
