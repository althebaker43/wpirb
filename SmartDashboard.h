#ifndef SMARTDASHBOARD_H
#define SMARTDASHBOARD_H

namespace frc
{
  class SmartDashboard
  {
  public:

    static void init();

    static bool PutBoolean(const char* key, bool value);
    static bool GetBoolean(const char* key, bool defaultValue);
  };
}; /* namespace frc */

#endif
