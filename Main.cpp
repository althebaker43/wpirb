
#include "FieldControlSystem.h"
#include "RedBot.h"
#include "RedBotPacket.h"
#include "IterativeRobot.h"
#include <iostream>
#include <argp.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define MAX_ERROR_COUNT 5

/**
 * Attempts to connect to the driver station via a socket
 */
int ConnectToDriverStation();

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
        frc::IterativeRobot& program
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

    std::cout << "Program: connecting to driver station." << std::endl;

    int socketFD = ConnectToDriverStation();
    if (socketFD < 0)
    {
        return 1;
    }

    std::cout << "Program: initializing program." << std::endl;
    program.RobotInit();
    if (InputOutputDevicePath.empty() == false)
    {
        robot = new RedBot(
                &program,
                InputOutputDevicePath.c_str(),
                new RedBotPacketGenerator()
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
                outputBuffer,
                new RedBotPacketGenerator()
                );
    }
    else
    {
        std::cerr << "Error: no communication devices specified." << std::endl;
        return 1;
    }

    FieldControlSystem::Mode robotMode = FieldControlSystem::MODE_DISABLED;
    char dsResponseBuf [1024];

    char dsRequestBuf [8];
    strcpy(dsRequestBuf, "request");
    dsRequestBuf[7] = '\0';

    robot->modeInit(robotMode);
    std::cout << "Program: beginning loop." << std::endl;
    size_t errorCount = 0;
    while (errorCount < MAX_ERROR_COUNT)
    {
        write(
                socketFD,
                (void*)dsRequestBuf,
                7
             );

        ssize_t dsResponseSize = recv(
                socketFD,
                (void*)dsResponseBuf,
                1024,
                0
                );
        if (dsResponseSize < 0)
        {
            std::cerr << "Error: no response from driver station." << std::endl;
            break;
        }
        else if (dsResponseSize > 0)
        {
            dsResponseBuf[dsResponseSize] = '\0';

            std::stringstream modeStream;
            int modeVal = 0;

            modeStream << dsResponseBuf;
            modeStream >> modeVal;

            FieldControlSystem::Mode newMode = robotMode;

            switch (modeVal)
            {
                case 0:
                    newMode = FieldControlSystem::MODE_DISABLED;
                    break;

                case 1:
                    newMode = FieldControlSystem::MODE_AUTO;
                    break;

                case 2:
                    newMode = FieldControlSystem::MODE_TELEOP;
                    break;

                case 3:
                    newMode = FieldControlSystem::MODE_TEST;
                    break;

                default:
                    break;
            };

            if (newMode != robotMode)
            {
                robot->modeInit(newMode);
                robotMode = newMode;
            }
        }

        robot->modePeriodic(robotMode);

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

int
ConnectToDriverStation()
{
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0)
    {
        error(0, errno, "Could not open socket file");
        return -1;
    }

    struct addrinfo getAddrHints;
    getAddrHints.ai_flags = 0;
    getAddrHints.ai_family = AF_INET;
    getAddrHints.ai_socktype = SOCK_STREAM;
    getAddrHints.ai_protocol = 0;
    getAddrHints.ai_addrlen = 0;
    getAddrHints.ai_addr = NULL;
    getAddrHints.ai_canonname = NULL;
    getAddrHints.ai_next = NULL;

    struct addrinfo* getAddrResults;

    int getAddrRetVal = getaddrinfo(
            "localhost",
            "9999",
            &getAddrHints,
            &getAddrResults
            );
    if (getAddrRetVal != 0)
    {
        std::cerr << "Error: could not get address information." << std::endl;
        return -1;
    }

    bool isConnected = false;
    for(
            struct addrinfo* addrInfoIter = getAddrResults;
            addrInfoIter != NULL;
            addrInfoIter = addrInfoIter->ai_next
       )
    {
        int connectRetVal = connect(
                socketFD,
                addrInfoIter->ai_addr,
                addrInfoIter->ai_addrlen
                );
        if (connectRetVal == 0)
        {
            isConnected = true;
            break;
        }
    }
    freeaddrinfo(getAddrResults);
    if (isConnected == false)
    {
        std::cerr << "Error: could not connect to driver station." << std::endl;
        return -1;
    }
    
    return socketFD;
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
