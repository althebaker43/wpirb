#ifndef SMARTDASHBOARD_H
#define SMARTDASHBOARD_H

// Forward declarations
namespace std
{
  template <class T> class shared_ptr;
}
namespace nt
{
  class NetworkTableInstance;
  class NetworkTable;
};
namespace llvm
{
  class StringRef;
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

    static bool PutBoolean(llvm::StringRef key, bool value);
    static bool GetBoolean(llvm::StringRef key, bool defaultValue);

    static bool PutNumber(llvm::StringRef key, double value);
    static double GetNumber(llvm::StringRef key, double defaultValue);

  private:

    static std::shared_ptr<nt::NetworkTable> GetTable();

    static nt::NetworkTableInstance ourNTInstance;
  };
}; /* namespace frc */

#endif
