#ifndef COMMAND_H
#define COMMAND_H

namespace frc
{
  class Subsystem;

  class Command
  {
  public:

    virtual ~Command(){}

    void Requires(Subsystem*);

    void Start();

    virtual void Initialize();
  };
}; /* namespace frc */

#endif /* ifndef COMMAND_H */
