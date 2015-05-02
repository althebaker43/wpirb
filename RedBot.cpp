
#include "RedBot.h"
#include "IterativeRobot.h"
#include <sstream>

#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>


RedBot::RedBot(
        IterativeRobot* program,
        const char*     deviceName
        ) :
    myProgram(program),
    myIsConnected(false),
    myDevice(NULL)
{
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

        myIsConnected = true;
    }
}

RedBot::~RedBot()
{
    if (myDevice != NULL)
    {
        fclose(myDevice);
    }
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
}


Packet*
Packet::Read(
        std::istream&   inputStream
        )
{
    if (inputStream.good() == false)
    {
        return NULL;
    }

    char header;
    inputStream.get(header);
    if (header == '\xFF')
    {
        Packet* packet;
        char packetType;
        inputStream.get(packetType);
        
        switch (packetType)
        {
            case '\x01':
                packet = new PingPacket();
                break;

            default:
                break;
        };

        if (packet != NULL)
        {
            inputStream >> *packet;
            if (packet->isValid())
            {
                return packet;
            }
            else
            {
                delete packet;
                return NULL;
            }
        }
    }

    return NULL;
}

bool
Packet::operator!=(
        const Packet&   packet
        ) const
{
    return !(*this == packet);
}

std::ostream&
operator<<(
        std::ostream&   outputStream,
        const Packet&   packet
        )
{
    packet.write(outputStream);
    return outputStream;
}

std::istream&
operator>>(
        std::istream&   inputStream,
        Packet&         packet
        )
{
    packet.read(inputStream);
    return inputStream;
}


PingPacket::PingPacket() :
    myIsValid(true)
{
}

void
PingPacket::write(
        std::ostream& outputStream
        ) const
{
    outputStream << "\xFF\x01\xFF";
}

void
PingPacket::read(
        std::istream& inputStream
        )
{
    unsigned char trailer;

    inputStream >> trailer;
    if (trailer == 0xFF)
    {
        myIsValid = true;
    }
    else
    {
        myIsValid = false;
    }
}

bool
PingPacket::isValid() const
{
    return myIsValid;
}

bool
PingPacket::operator==(
        const Packet&   packet
        ) const
{
    if (getType() == packet.getType())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Packet::Type
PingPacket::getType() const
{
    return TYPE_PING;
}

PingPacket::operator std::string() const
{
    std::ostringstream stringStream;
    write(stringStream);
    return stringStream.str();
}
