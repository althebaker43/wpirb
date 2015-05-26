
#include "FieldControlSystem.h"
#include "RedBot.h"
#include <iostream>

#define MAX_ERROR_COUNT 5

/**
 * Writes an error message for an invalid received packet
 */
void WriteIncoherentMessage(
        const RedBot& robot
        );

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
    size_t errorCount = 0;
    while (errorCount < MAX_ERROR_COUNT)
    {
        robot.modePeriodic(FieldControlSystem::MODE_DISABLED);

        if (robot.getStatus() != RedBot::STATUS_GOOD)
        {
            switch (robot.getStatus())
            {
                case RedBot::STATUS_DISCONNECTED:
                    std::cerr << "Error: robot lost connection." << std::endl;
                    break;

                case RedBot::STATUS_INCOHERENT:
                    WriteIncoherentMessage(robot);
                    robot.resync();
                    break;

                case RedBot::STATUS_UNRESPONSIVE:
                    std::cerr << "Error: robot is unresponsive." << std::endl;
                    robot.resync();
                    break;

                default:
                    break;
            }

            ++errorCount;
        }
        else
        {
            // Reset error count
            errorCount = 0;
        }
    }

    return (errorCount >= MAX_ERROR_COUNT);
}

void
WriteIncoherentMessage(
        const RedBot& robot
        )
{
    std::list<std::string> lastRequestPackets;
    std::list<std::string> lastResponsePackets;

    robot.getLastBinaryTransaction(
            lastRequestPackets,
            lastResponsePackets
            );
    const std::string& invalidPacket = lastResponsePackets.back();

    std::cerr << "Error: invalid packet received: ";

    for(
            size_t charIdx = 0;
            charIdx < invalidPacket.length();
            ++charIdx
       )
    {
        std::cerr << std::hex << (unsigned int)invalidPacket[0] << ' ';
    }

    std::cerr << std::endl;
}
