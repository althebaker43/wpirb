#ifndef WPILIB_H
#define WPILIB_H

#include <stdlib.h>

// Framework
#include "IterativeRobot.h"

// Components
#include "DigitalOutput.h"
#include "Timer.h"
#include "RobotDrive.h"
#include "Joystick.h"
#include "LiveWindow.h"

#include "FieldControlSystem.h"
#include "RedBot.h"

#define START_ROBOT_CLASS(RobotClass) \
    int main(int argc, char** argv) { \
        RobotClass program; \
        RedBot robot(&program, (const char*)NULL); \
        FieldControlSystem* fcs = FieldControlSystem::GetInstance(); \
        fcs->Register(&robot); \
        FieldControlSystem::DestroyInstance(); \
        LiveWindow::DestroyInstance(); \
        return 0; \
    }

#endif /* ifndef WPILIB_H */
