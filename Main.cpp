
#include "FieldControlSystem.h"
#include "RedBot.h"
#include <iostream>


int
WPIRBMain(
        int             argc,
        char**          argv,
        IterativeRobot& program
        )
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <device-name>" << std::endl;
        return 1;
    }

    std::cout << "Program: initializing program." << std::endl;
    RedBot robot(
            &program,
            argv[1]
            );

    robot.modeInit(FieldControlSystem::MODE_DISABLED);
    std::cout << "Program: beginning loop." << std::endl;
    int status = 0;
    while(true)
    {
        robot.modePeriodic(FieldControlSystem::MODE_DISABLED);
        if (robot.isConnected() == false)
        {
            std::cerr << "Error: robot lost connection." << std::endl;
            status = 1;
            break;
        }
    }

    return status;
}
