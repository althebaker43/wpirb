
#include "RedBot.h"
#include "IterativeRobot.h"
#include "Packet.h"
#include "Component.h"
#include <sstream>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>


RedBot::Components RedBot::ourCurrentComponents;


void
RedBot::RegisterComponent(
        Component* component
        )
{
    ourCurrentComponents.push_back(component);
}

void
RedBot::ClearRegisteredComponents()
{
    ourCurrentComponents.clear();
}

RedBot::RedBot(
        IterativeRobot* program,
        const char*     deviceName
        ) :
    myProgram(program),
    myIsConnected(false),
    myDevice(NULL),
    myInputBuffer(NULL),
    myOutputBuffer(NULL)
{
    // Move all newly registered components to my own collection
    myComponents = ourCurrentComponents;
    ourCurrentComponents.clear();

    if (deviceName != NULL)
    {
        // Open device

        int deviceFD = open(deviceName, O_RDONLY);
        if (deviceFD == -1)
        {
            error(0, errno, "Could not open %s", deviceName);
            return;
        }

        myDevice = fdopen(deviceFD, "rb");
        if (myDevice == NULL)
        {
            error(0, errno, "Could not open %s", deviceName);
            return;
        }

        // Configure device

        struct termios deviceConfig;
        struct termios actualDeviceConfig;

        tcgetattr(deviceFD, &deviceConfig);
        cfsetspeed(&deviceConfig, OUR_DEV_SPEED);
        if (tcsetattr(deviceFD, 0, &deviceConfig) == -1)
        {
            error(0, errno, "Could not configure device %s", deviceName);
            return;
        }

        tcgetattr(deviceFD, &actualDeviceConfig);
        if(
                (cfgetispeed(&actualDeviceConfig) != OUR_DEV_SPEED) ||
                (cfgetospeed(&actualDeviceConfig) != OUR_DEV_SPEED)
          )
        {
            close(deviceFD);
            error(0, 0, "Could not configure device %s", deviceName);
            return;
        }

        myInputBuffer = new InputFileBuffer(myDevice);
        myOutputBuffer = new OutputFileBuffer(myDevice);
        myIsConnected = true;
    }
}

RedBot::RedBot(
        IterativeRobot* program,
        FILE*           device
        ) :
    myProgram(program),
    myIsConnected(true),
    myDevice(device),
    myInputBuffer(new InputFileBuffer(device)),
    myOutputBuffer(new OutputFileBuffer(device))
{
    // Move all newly registered components to my own collection
    myComponents = ourCurrentComponents;
    ourCurrentComponents.clear();
}

RedBot::RedBot(
        IterativeRobot* program,
        InputBuffer*    inputBuffer,
        OutputBuffer*   outputBuffer
        ) :
    myProgram(program),
    myIsConnected(true),
    myDevice(NULL),
    myInputBuffer(inputBuffer),
    myOutputBuffer(outputBuffer)
{
    // Move all newly registered components to my own collection
    myComponents = ourCurrentComponents;
    ourCurrentComponents.clear();
}

RedBot::~RedBot()
{
    // Close the serial device
    if (myDevice != NULL)
    {
        fclose(myDevice);
    }

    // Discard the unused incoming packets
    while (myIncomingPackets.empty() == false)
    {
        delete (myIncomingPackets.front());
        myIncomingPackets.pop();
    }

    delete myInputBuffer;
    delete myOutputBuffer;
}

bool
RedBot::isConnected() const
{
    return myIsConnected;
}

void
RedBot::modeInit(
        FieldControlSystem::Mode mode
        )
{
    if (isConnected() == false)
    {
        return;
    }

    switch (mode)
    {
        case FieldControlSystem::MODE_DISABLED:
            myProgram->DisabledInit();
            break;

        case FieldControlSystem::MODE_AUTO:
            myProgram->AutonomousInit();
            break;

        case FieldControlSystem::MODE_TELEOP:
            myProgram->TeleopInit();
            break;

        case FieldControlSystem::MODE_TEST:
            myProgram->TestInit();
            break;

        default:
            break;
    };
}

void
RedBot::modePeriodic(
        FieldControlSystem::Mode mode
        )
{
    if (isConnected() == false)
    {
        return;
    }

    // Process incoming packets
    while (myIncomingPackets.empty() == false)
    {
        Packet* packet = myIncomingPackets.front();
        myIncomingPackets.pop();

        for(
                Components::const_iterator compIter = myComponents.begin();
                compIter != myComponents.end();
                ++compIter
           )
        {
            Component* component = *compIter;

            if (component->processPacket(*packet) == true)
            {
                break;
            }
        }

        delete packet;
    }

    switch (mode)
    {
        case FieldControlSystem::MODE_DISABLED:
            myProgram->DisabledPeriodic();
            break;

        case FieldControlSystem::MODE_AUTO:
            myProgram->AutonomousPeriodic();
            break;

        case FieldControlSystem::MODE_TELEOP:
            myProgram->TeleopPeriodic();
            break;

        case FieldControlSystem::MODE_TEST:
            myProgram->TestPeriodic();
            break;

        default:
            break;
    };

    // Send outgoing packets
    for(
            Components::const_iterator compIter = myComponents.begin();
            compIter != myComponents.end();
            ++compIter
       )
    {
        Component* component = *compIter;
        Packet* outPacket = component->getNextPacket();

        if (outPacket == NULL)
        {
            continue;
        }

        outPacket->write(myOutputBuffer->getContents());
        delete outPacket;

        while (myOutputBuffer->write() == true);
        myOutputBuffer->clear();

        myInputBuffer->clear();
        while (myInputBuffer->read() == true);
        Packet* inPacket = Packet::Read(myInputBuffer->getContents());

        if (inPacket == NULL)
        {
            continue;
        }

        myIncomingPackets.push(inPacket);
    }
}
