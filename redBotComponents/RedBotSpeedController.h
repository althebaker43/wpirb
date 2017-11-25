
#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include <stdlib.h>

namespace frc
{

  class SpeedController
  {
  };

  class PWMSpeedController : public SpeedController
  {
  };

}; /* namespace frc */

class RedBotSpeedController : public frc::PWMSpeedController
{
 public:

  RedBotSpeedController(size_t channel);
};

#endif /* ifndef SPEEDCONTROLLER_H */
