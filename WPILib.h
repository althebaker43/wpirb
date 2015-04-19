#ifndef WPILIB_H
#define WPILIB_H

#include <stdlib.h>

#include "IterativeRobot.h"
#include "RobotDrive.h"
#include "Joystick.h"
#include "LiveWindow.h"

#include "FieldControlSystem.h"

#define START_ROBOT_CLASS(RobotClass) \
    int main(int argc, char** argv) { \
        RobotClass robot = RobotClass(); \
        FieldControlSystem* fcs = FieldControlSystem::GetInstance(); \
        fcs->Register(&robot); \
        FieldControlSystem::DestroyInstance(); \
        LiveWindow::DestroyInstance(); \
        return 0; \
    }

#endif /* ifndef WPILIB_H */
