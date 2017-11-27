#ifndef SMARTDASHBOARD_H
#define SMARTDASHBOARD_H

namespace nt
{
  class NetworkTableInstance;
};

namespace frc
{
  class SmartDashboard
  {
  public:

    static void init();

    /**
     * One-time initialization given non-default NT instance
     *
     * Primarily used for testing purposes.
     */
    static void init(nt::NetworkTableInstance ntInst);

    static bool PutBoolean(const char* key, bool value);
    static bool GetBoolean(const char* key, bool defaultValue);

  private:

    static nt::NetworkTableInstance ourNTInstance;
  };
}; /* namespace frc */

#endif
