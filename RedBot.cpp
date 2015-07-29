
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
    myStatus(STATUS_DISCONNECTED),
    myIsUsingExternalBuffers(false),
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

        myDevice = fopen(deviceName, "r+");
        if (myDevice == NULL)
        {
            error(0, errno, "Could not open %s", deviceName);
            return;
        }

        myInputBuffer = new InputFileBuffer(myDevice);
        myOutputBuffer = new OutputFileBuffer(myDevice);
        myStatus = STATUS_GOOD;
    }
}

RedBot::RedBot(
        IterativeRobot* program,
        FILE*           device
        ) :
    myProgram(program),
    myStatus(STATUS_GOOD),
    myIsUsingExternalBuffers(false),
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
    myStatus(STATUS_GOOD),
    myIsUsingExternalBuffers(true),
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

    if (myIsUsingExternalBuffers == false)
    {
        delete myInputBuffer;
        delete myOutputBuffer;
    }
}

bool
RedBot::isConnected() const
{
    return (myStatus != STATUS_DISCONNECTED);
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

    // Run user's periodic function
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

    // Exchange data with the robot
    transferData();
}

void
RedBot::transferData()
{
    if(
            (myInputBuffer == NULL) ||
            (myOutputBuffer == NULL)
      )
    {
        return;
    }

    PingPacket pingPacket;

    // Confirm connectivity to robot
    unsigned int initialPingCount = 0;
    do
    {
        Packet* inPacket;

        exchangePackets(
                &pingPacket,
                inPacket
                );

        if (inPacket != NULL)
        {
            myIncomingPackets.push(inPacket);
        }
    }
    while(
            (myStatus != STATUS_GOOD) &&
            ((++initialPingCount) < 5)
         );
    if (initialPingCount >= 5)
    {
        return;
    }

    std::queue<Packet*> outgoingPackets;

    myLastTransactionSentData.clear();
    myLastTransactionReceivedData.clear();

    // Collect outgoing packets from components
    for(
            Components::const_iterator compIter = myComponents.begin();
            compIter != myComponents.end();
            ++compIter
       )
    {
        Component* component = *compIter;

        for(
                size_t packetCount = 0;
                packetCount < 10;
                ++packetCount
           )
        {
            Packet* outPacket = component->getNextPacket();
            if (outPacket == NULL)
            {
                break;
            }

            outgoingPackets.push(outPacket);
        }
    }

    // Send outgoing packets
    while (outgoingPackets.empty() == false)
    {
        Packet* outPacket = outgoingPackets.front();
        Packet* inPacket = NULL;

        outgoingPackets.pop();

        exchangePackets(
                outPacket,
                inPacket
                );
        delete outPacket;
        if (inPacket == NULL)
        {
            continue;
        }

        myIncomingPackets.push(inPacket);
    }

    // Continue pinging until no more incoming packets to process
    while (true)
    {
        Packet* inPacket = NULL;

        exchangePackets(
                &pingPacket,
                inPacket
                );
        if (inPacket == NULL)
        {
            break;
        }

        if (inPacket->getType() == Packet::TYPE_ACK)
        {
            delete inPacket;
            break;
        }
        else
        {
            myIncomingPackets.push(inPacket);
        }
    }
}

void
RedBot::exchangePackets(
        Packet*     requestPacket,
        Packet*&    responsePacket
        )
{
    std::ostringstream outgoingPacketStream;
    std::string incomingPacketData;

    requestPacket->write(myOutputBuffer->getOutputStream());
    myOutputBuffer->writePacket();
    myOutputBuffer->clear();

    // Record sent data
    outgoingPacketStream << *requestPacket;
    myLastTransactionSentData.push_back(outgoingPacketStream.str());

    myInputBuffer->clear();
    myInputBuffer->readPacket();
    responsePacket = Packet::Read(
            myInputBuffer->getInputStream(),
            &incomingPacketData // Record received data
            );

    myLastTransactionReceivedData.push_back(incomingPacketData);

    // Decide on robot status
    if (responsePacket == NULL)
    {
        if (incomingPacketData.empty() == true)
        {
            myStatus = STATUS_UNRESPONSIVE;
        }
        else
        {
            myStatus = STATUS_INCOHERENT;
        }
    }
    else
    {
        myStatus = STATUS_GOOD;
    }
}

RedBot::Status
RedBot::getStatus() const
{
    return myStatus;
}

void
RedBot::getLastBinaryTransaction(
        std::list<std::string>& sentData,
        std::list<std::string>& receivedData
        ) const
{
    sentData = myLastTransactionSentData;
    receivedData = myLastTransactionReceivedData;
}

void
RedBot::resync()
{
    // Send resync sequence
    myOutputBuffer->resync();

    // Discard any incoming data
    myInputBuffer->readPacket();
    myInputBuffer->clear();
}
