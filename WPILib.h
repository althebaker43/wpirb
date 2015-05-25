#ifndef WPILIB_H
#define WPILIB_H

// Framework
#include "IterativeRobot.h"

// Components
#include "DigitalOutput.h"
#include "Timer.h"
#include "RobotDrive.h"
#include "Joystick.h"
#include "LiveWindow.h"

// Program entry point
#include "Main.h"

#define START_ROBOT_CLASS(RobotClass) \
    int main(int argc, char** argv) { \
        RobotClass program; \
        return WPIRBMain(argc, argv, program); \
    }

#endif /* ifndef WPILIB_H */
