#ifndef SMARTDASHBOARD_H
#define SMARTDASHBOARD_H

#include <string>
#include <memory>

// Forward declarations
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

    static bool PutString(llvm::StringRef key, llvm::StringRef value);
    static std::string GetString(llvm::StringRef key, llvm::StringRef defaultValue);

  private:

    static std::shared_ptr<nt::NetworkTable> GetTable();

    static nt::NetworkTableInstance ourNTInstance;
  };
}; /* namespace frc */

#endif
