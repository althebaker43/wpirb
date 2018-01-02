#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>

namespace frc
{
  class Subsystem
  {
  public:

    Subsystem(const std::string&);

    virtual ~Subsystem(){}
  };
}; /* namespace frc */

#endif /* ifndef SUBSYSTEM_H */
