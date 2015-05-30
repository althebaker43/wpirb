
#include "FieldControlSystem.h"
#include "RedBot.h"
#include <iostream>
#include <argp.h>
#include <errno.h>
#include <error.h>

#define MAX_ERROR_COUNT 5

/**
 * Writes an error message for an invalid received packet
 */
void WriteIncoherentMessage(
        const RedBot& robot
        );

/**
 * Parses arguments passed to program
 */
error_t ArgumentParser(
        int                 key,
        char*               arg,
        struct argp_state*  state
        );

/**
 * Array of options that can be passed to the base-station program
 */
static struct argp_option options [] = {
    {
        "device",
        'd',
        "device-path",
        0,
        "Path to serial device file that robot is connected to"
    },
    {
        "input-device",
        'i',
        "input-device-path",
        0,
        "Path to input serial device that robot sends its output to"
    },
    {
        "output-device",
        'o',
        "output-device-path",
        0,
        "Path to output serial device that robot receives its input from"
    },
    0
};

/**
 * Base-station program argument parser configuration
 */
static struct argp parserConfig = {
    options,
    ArgumentParser,
    NULL,
    "Base station program to control RedBot",
    NULL,
    NULL,
    NULL
};

/**
 * Path of serial I/O device that robot is connected to
 */
static std::string InputOutputDevicePath;

/**
 * Path of input device that robot sends output to
 */
static std::string InputDevicePath;

/**
 * Path of output device that robot receives input from
 */
static std::string OutputDevicePath;

int
WPIRBMain(
        int             argc,
        char**          argv,
        IterativeRobot& program
        )
{
    RedBot* robot = NULL;
    InputFileBuffer* inputBuffer = NULL;
    OutputFileBuffer* outputBuffer = NULL;

    argp_parse(
            &parserConfig,
            argc,
            argv,
            0,
            NULL,
            NULL
            );

    std::cout << "Program: initializing program." << std::endl;
    if (InputOutputDevicePath.empty() == false)
    {
        robot = new RedBot(
                &program,
                InputOutputDevicePath.c_str()
                );
    }
    else if(
            (InputDevicePath.empty() == false) &&
            (OutputDevicePath.empty() == false)
           )
    {
        FILE* inputDevice = fopen(InputDevicePath.c_str(), "r");
        if (inputDevice == NULL)
        {
            error(
                    1,
                    errno,
                    "Could not open input device %s",
                    InputDevicePath.c_str()
                 );
        }
        inputBuffer = new InputFileBuffer(inputDevice);

        FILE* outputDevice = fopen(OutputDevicePath.c_str(), "w");
        if (outputDevice == NULL)
        {
            error(
                    1,
                    errno,
                    "Could not open output device %s",
                    OutputDevicePath.c_str()
                 );
        }
        outputBuffer = new OutputFileBuffer(outputDevice);

        robot = new RedBot(
                &program,
                inputBuffer,
                outputBuffer
                );
    }
    else
    {
        std::cerr << "Error: no communication devices specified." << std::endl;
        return 1;
    }

    robot->modeInit(FieldControlSystem::MODE_DISABLED);
    std::cout << "Program: beginning loop." << std::endl;
    size_t errorCount = 0;
    while (errorCount < MAX_ERROR_COUNT)
    {
        robot->modePeriodic(FieldControlSystem::MODE_DISABLED);

        if (robot->getStatus() != RedBot::STATUS_GOOD)
        {
            switch (robot->getStatus())
            {
                case RedBot::STATUS_DISCONNECTED:
                    std::cerr << "Error: robot lost connection." << std::endl;
                    break;

                case RedBot::STATUS_INCOHERENT:
                    WriteIncoherentMessage(*robot);
                    robot->resync();
                    break;

                case RedBot::STATUS_UNRESPONSIVE:
                    std::cerr << "Error: robot is unresponsive." << std::endl;
                    robot->resync();
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

    delete robot;
    delete inputBuffer;
    delete outputBuffer;

    return (errorCount >= MAX_ERROR_COUNT);
}

error_t
ArgumentParser(
        int                 key,
        char*               arg,
        struct argp_state*  state
        )
{
    error_t status = 0;

    switch (key)
    {
        case 'd':
            InputOutputDevicePath = arg;
            break;

        case 'i':
            InputDevicePath = arg;
            break;

        case 'o':
            OutputDevicePath = arg;
            break;

        default:
            status = ARGP_ERR_UNKNOWN;
            break;
    };

    return status;
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
        std::cerr << std::hex << "0x" << (0xFF & (unsigned int)invalidPacket[0]) << ' ';
    }

    std::cerr << std::endl;
}
