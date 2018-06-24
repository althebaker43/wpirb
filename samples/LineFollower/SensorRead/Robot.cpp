
#include <WPILib.h>
#include <llvm/StringRef.h>

class Robot : public frc::IterativeRobot
{
private:

  frc::AnalogInput myLeftSensor;
  frc::AnalogInput myMiddleSensor;
  frc::AnalogInput myRightSensor;

public:

  Robot() :
    myLeftSensor(3),
    myMiddleSensor(6),
    myRightSensor(7)
  {
    frc::SmartDashboard::init();
  }

  void DisabledInit()
  {
  }

  void AutonomousInit()
  {
  }

  void AutonomousPeriodic()
  {
    frc::SmartDashboard::PutNumber("Left Sensor", myLeftSensor.Get());
    frc::SmartDashboard::PutNumber("Middle Sensor", myMiddleSensor.Get());
    frc::SmartDashboard::PutNumber("Right Sensor", myRightSensor.Get());
  }
};

START_ROBOT_CLASS(Robot);
