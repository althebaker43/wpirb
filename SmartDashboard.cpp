
#include "SmartDashboard.h"
#include "networktables/NetworkTableInstance.h"

using namespace frc;


void
SmartDashboard::init()
{
  nt::NetworkTableInstance::GetDefault().StartServer();
}

bool
SmartDashboard::PutBoolean(const char* key, bool value)
{
  return nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard")->PutBoolean(key, value);
}

bool
SmartDashboard::GetBoolean(const char* key, bool defaultValue)
{
  return nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard")->GetBoolean(key, defaultValue);
}
