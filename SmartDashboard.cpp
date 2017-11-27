
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
SmartDashboard::PutBoolean(const char* key, bool value)
{
  return ourNTInstance.GetTable("SmartDashboard")->PutBoolean(key, value);
}

bool
SmartDashboard::GetBoolean(const char* key, bool defaultValue)
{
  return ourNTInstance.GetTable("SmartDashboard")->GetBoolean(key, defaultValue);
}
