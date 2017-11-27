
#include "SmartDashboard.h"
#include "networktables/NetworkTableInstance.h"

using namespace frc;


nt::NetworkTableInstance frc::SmartDashboard::ourNTInstance;

void
SmartDashboard::init()
{
  ourNTInstance = nt::NetworkTableInstance::GetDefault();
  ourNTInstance.StartServer();
}

void
SmartDashboard::init(nt::NetworkTableInstance ntInst)
{
  ourNTInstance = ntInst;
  ourNTInstance.StartServer();
}

bool
SmartDashboard::PutBoolean(llvm::StringRef key, bool value)
{
  return GetTable()->PutBoolean(key, value);
}

bool
SmartDashboard::GetBoolean(llvm::StringRef key, bool defaultValue)
{
  return GetTable()->GetBoolean(key, defaultValue);
}

bool
SmartDashboard::PutNumber(llvm::StringRef key, double value)
{
  return GetTable()->PutNumber(key, value);
}

double
SmartDashboard::GetNumber(llvm::StringRef key, double defaultValue)
{
  return GetTable()->GetNumber(key, defaultValue);
}

bool
SmartDashboard::PutString(llvm::StringRef key, llvm::StringRef value)
{
  return GetTable()->PutString(key, value);
}

std::string
SmartDashboard::GetString(llvm::StringRef key, llvm::StringRef defaultValue)
{
  return GetTable()->GetString(key, defaultValue);
}

std::shared_ptr<NetworkTable>
SmartDashboard::GetTable()
{
  return ourNTInstance.GetTable("SmartDashboard");
}
