
#include "TestComponents.h"
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "AnalogInput.h"
#include "RobotDrive.h"


static void CheckDrive(
        RobotDrive&             drive,
        std::list<Packet*>&     packets,
        double                  magnitude,
        double                  curve,
        const MotorDrivePacket& rightPacket,
        const MotorDrivePacket& leftPacket
        );

template <class RequestType> static void CheckConfiguration(
        Component&                          component,
        unsigned int                        pin,
        PinConfigInfoPacket::PinDirection   direction,
        std::list<Packet*>&                 packets
        );


TEST(Components, DigitalOutputConfigTest)
{
    frc::DigitalOutput dOut(9);
    dOut.Set(1);

    CheckConfiguration<DigitalOutputPacket>(
            dOut,
            9,
            PinConfigInfoPacket::DIR_OUTPUT,
            myPackets
            );
}

TEST(Components, DigitalOutputTest)
{
    frc::DigitalOutput dOut(5);
    dOut.Set(1);
    Packet* packet0 = dOut.getNextPacket();
    myPackets.push_back(packet0);

    CHECK(packet0 != NULL);
    CHECK(NULL != dynamic_cast<PinConfigPacket*>(packet0));

    PinConfigPacket* pinConfigPacket = static_cast<PinConfigPacket*>(packet0);

    CHECK_EQUAL(5, pinConfigPacket->getPin());
    CHECK_EQUAL(PinConfigPacket::DIR_OUTPUT, pinConfigPacket->getDirection());

    dOut.processPacket(PinConfigInfoPacket(5, PinConfigInfoPacket::DIR_OUTPUT));

    Packet* packet1 = dOut.getNextPacket();
    myPackets.push_back(packet1);

    CHECK(packet1 != NULL);
    CHECK(NULL != dynamic_cast<DigitalOutputPacket*>(packet1));

    DigitalOutputPacket* dOutPacket1 = static_cast<DigitalOutputPacket*>(packet1);

    CHECK_EQUAL(5, dOutPacket1->getPin());
    CHECK_EQUAL(true, dOutPacket1->getValue());
    POINTERS_EQUAL(NULL, dOut.getNextPacket());

    dOut.Set(0);
    dOut.Set(1);
    Packet* packet2 = dOut.getNextPacket();
    myPackets.push_back(packet2);

    CHECK(packet2 != NULL);
    CHECK(NULL != dynamic_cast<DigitalOutputPacket*>(packet2));

    DigitalOutputPacket* dOutPacket2 = static_cast<DigitalOutputPacket*>(packet2);

    CHECK_EQUAL(5, dOutPacket2->getPin());
    CHECK_EQUAL(true, dOutPacket2->getValue());
}

TEST(Components, DigitalInputConfigTest)
{
    frc::DigitalInput dIn(9);

    CheckConfiguration<DigitalInputPacket>(
            dIn,
            9,
            PinConfigInfoPacket::DIR_INPUT,
            myPackets
            );
}

TEST(Components, DigitalInputTest)
{
    frc::DigitalInput dIn(4);

    Packet* packet0 = dIn.getNextPacket();
    myPackets.push_back(packet0);

    CHECK(packet0 != NULL);
    CHECK(NULL != dynamic_cast<PinConfigPacket*>(packet0));

    PinConfigPacket* configPacket = static_cast<PinConfigPacket*>(packet0);

    CHECK_EQUAL(4, configPacket->getPin());
    CHECK_EQUAL(PinConfigPacket::DIR_INPUT, configPacket->getDirection());

    dIn.processPacket(PinConfigInfoPacket(4, PinConfigInfoPacket::DIR_INPUT));

    Packet* packet1 = dIn.getNextPacket();
    myPackets.push_back(packet1);

    CHECK(packet1 != NULL);
    CHECK(NULL != dynamic_cast<DigitalInputPacket*>(packet1));

    DigitalInputPacket* dInPacket1 = static_cast<DigitalInputPacket*>(packet1);

    CHECK_EQUAL(4, dInPacket1->getPin());

    DigitalValuePacket dValPacket1(4, false);

    CHECK(dIn.processPacket(dValPacket1));
    CHECK_FALSE(dIn.Get());

    DigitalValuePacket dValPacket2(4, true);

    CHECK(dIn.processPacket(dValPacket2));
    CHECK_TRUE(dIn.Get());

    DigitalValuePacket dValPacket3(5, false);

    CHECK_FALSE(dIn.processPacket(dValPacket3));
    CHECK_TRUE(dIn.Get());
}

TEST(Components, AnalogInputTest)
{
    frc::AnalogInput aIn(3);

    Packet* packet1 = aIn.getNextPacket();
    myPackets.push_back(packet1);

    CHECK(packet1 != NULL);
    CHECK(NULL != dynamic_cast<AnalogInputPacket*>(packet1));
}

TEST(Components, RobotDriveSimpleTest)
{
    RobotDrive drive(0, 0);
    myPackets.push_back(drive.getNextPacket());

    CHECK_EQUAL((Packet*)NULL, myPackets.back());

    drive.Drive(0.0, 0.0);
    myPackets.push_back(drive.getNextPacket());

    CHECK(NULL != myPackets.back());
    CHECK(NULL != dynamic_cast<MotorDrivePacket*>(myPackets.back()));

    MotorDrivePacket* mDrivePacket1 = static_cast<MotorDrivePacket*>(myPackets.back());

    CHECK_EQUAL(0, mDrivePacket1->getSpeed());
    CHECK_EQUAL(MotorDrivePacket::DIR_FORWARD, mDrivePacket1->getDirection());

    myPackets.push_back(drive.getNextPacket());

    CHECK(NULL != myPackets.back());
    CHECK(NULL != dynamic_cast<MotorDrivePacket*>(myPackets.back()));

    MotorDrivePacket* mDrivePacket2 = static_cast<MotorDrivePacket*>(myPackets.back());

    CHECK_EQUAL(0, mDrivePacket2->getSpeed());
    CHECK_EQUAL(MotorDrivePacket::DIR_FORWARD, mDrivePacket2->getDirection());

    if (mDrivePacket1->getMotor() == MotorDrivePacket::MOTOR_RIGHT)
    {
        CHECK_EQUAL(MotorDrivePacket::MOTOR_LEFT, mDrivePacket2->getMotor())
    }
    else
    {
        CHECK_EQUAL(MotorDrivePacket::MOTOR_RIGHT, mDrivePacket2->getMotor())
    }

    myPackets.push_back(drive.getNextPacket());

    CHECK_EQUAL((Packet*)NULL, myPackets.back());
}

TEST(Components, RobotDriveStraightTest)
{
    RobotDrive drive(0, 0);
    MotorDrivePacket* mDrivePacket1;
    MotorDrivePacket* mDrivePacket2;

    drive.Drive(1.0, 0.0);

    myPackets.push_back(drive.getNextPacket());
    mDrivePacket1 = dynamic_cast<MotorDrivePacket*>(myPackets.back());
    myPackets.push_back(drive.getNextPacket());
    mDrivePacket2 = dynamic_cast<MotorDrivePacket*>(myPackets.back());

    CHECK(NULL != mDrivePacket1);
    CHECK(NULL != mDrivePacket2);
    CHECK_EQUAL(255, mDrivePacket1->getSpeed());
    CHECK_EQUAL(255, mDrivePacket2->getSpeed());
    CHECK_EQUAL(MotorDrivePacket::DIR_FORWARD, mDrivePacket1->getDirection());
    CHECK_EQUAL(MotorDrivePacket::DIR_FORWARD, mDrivePacket2->getDirection());

    drive.Drive(-1.0, 0.0);

    myPackets.push_back(drive.getNextPacket());
    mDrivePacket1 = dynamic_cast<MotorDrivePacket*>(myPackets.back());
    myPackets.push_back(drive.getNextPacket());
    mDrivePacket2 = dynamic_cast<MotorDrivePacket*>(myPackets.back());

    CHECK(NULL != mDrivePacket1);
    CHECK(NULL != mDrivePacket2);
    CHECK_EQUAL(255, mDrivePacket1->getSpeed());
    CHECK_EQUAL(255, mDrivePacket2->getSpeed());
    CHECK_EQUAL(MotorDrivePacket::DIR_BACKWARD, mDrivePacket1->getDirection());
    CHECK_EQUAL(MotorDrivePacket::DIR_BACKWARD, mDrivePacket2->getDirection());

    drive.Drive(0.5, 0.0);

    myPackets.push_back(drive.getNextPacket());
    mDrivePacket1 = dynamic_cast<MotorDrivePacket*>(myPackets.back());
    myPackets.push_back(drive.getNextPacket());
    mDrivePacket2 = dynamic_cast<MotorDrivePacket*>(myPackets.back());

    CHECK(NULL != mDrivePacket1);
    CHECK(NULL != mDrivePacket2);
    CHECK_EQUAL(127, mDrivePacket1->getSpeed());
    CHECK_EQUAL(127, mDrivePacket2->getSpeed());
    CHECK_EQUAL(MotorDrivePacket::DIR_FORWARD, mDrivePacket1->getDirection());
    CHECK_EQUAL(MotorDrivePacket::DIR_FORWARD, mDrivePacket2->getDirection());

    drive.Drive(1.5, 0.0);

    myPackets.push_back(drive.getNextPacket());
    mDrivePacket1 = dynamic_cast<MotorDrivePacket*>(myPackets.back());
    myPackets.push_back(drive.getNextPacket());
    mDrivePacket2 = dynamic_cast<MotorDrivePacket*>(myPackets.back());

    CHECK_EQUAL((MotorDrivePacket*)NULL, mDrivePacket1);
    CHECK_EQUAL((MotorDrivePacket*)NULL, mDrivePacket2);
}

TEST(Components, CurveDriveTest)
{
    RobotDrive drive(0, 0);

    // Stationary
    CheckDrive(
            drive,
            myPackets,
            0.0,
            0.0,
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                0,
                MotorDrivePacket::DIR_FORWARD
                ),
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                0,
                MotorDrivePacket::DIR_FORWARD
                )
            );

    // Drive forward
    CheckDrive(
            drive,
            myPackets,
            1.0,
            0.0,
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                255,
                MotorDrivePacket::DIR_FORWARD
                ),
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                255,
                MotorDrivePacket::DIR_FORWARD
                )
            );

    // Drive backward
    CheckDrive(
            drive,
            myPackets,
            -1.0,
            0.0,
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                255,
                MotorDrivePacket::DIR_BACKWARD
                ),
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                255,
                MotorDrivePacket::DIR_BACKWARD
                )
            );

    // Pivot right
    CheckDrive(
            drive,
            myPackets,
            0.0,
            1.0,
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                255,
                MotorDrivePacket::DIR_BACKWARD
                ),
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                255,
                MotorDrivePacket::DIR_FORWARD
                )
            );

    // Pivot left
    CheckDrive(
            drive,
            myPackets,
            0.0,
            -1.0,
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_RIGHT,
                255,
                MotorDrivePacket::DIR_FORWARD
                ),
            MotorDrivePacket(
                MotorDrivePacket::MOTOR_LEFT,
                255,
                MotorDrivePacket::DIR_BACKWARD
                )
            );
}

void
CheckDrive(
        RobotDrive&             drive,
        std::list<Packet*>&     packets,
        double                  magnitude,
        double                  curve,
        const MotorDrivePacket& expRightPacket,
        const MotorDrivePacket& expLeftPacket
        )
{
    drive.Drive(
            magnitude,
            curve
            );

    bool rightDrivePacketFound = false;
    bool leftDrivePacketFound = false;

    packets.push_back(drive.getNextPacket());
    packets.push_back(drive.getNextPacket());

    std::list<Packet*>::const_reverse_iterator packetIter = packets.rbegin();

    for (int i = 0; i < 2; i++)
    {
        Packet* packet = *(packetIter++);
        MotorDrivePacket* mDrivePacket = dynamic_cast<MotorDrivePacket*>(packet);

        CHECK(NULL != mDrivePacket);

        if (mDrivePacket->getMotor() == MotorDrivePacket::MOTOR_RIGHT)
        {
            rightDrivePacketFound = true;
            CHECK_EQUAL(expRightPacket.getSpeed(), mDrivePacket->getSpeed());
            CHECK_EQUAL(expRightPacket.getDirection(), mDrivePacket->getDirection());
        }
        else
        {
            leftDrivePacketFound = true;
            CHECK_EQUAL(expLeftPacket.getSpeed(), mDrivePacket->getSpeed());
            CHECK_EQUAL(expLeftPacket.getDirection(), mDrivePacket->getDirection());
        }
    }

    CHECK(rightDrivePacketFound);
    CHECK(leftDrivePacketFound);
}

template <class RequestType>
void
CheckConfiguration(
        Component&                          component,
        unsigned int                        pin,
        PinConfigInfoPacket::PinDirection   direction,
        std::list<Packet*>&                 packets
        )
{
    Packet* requestPacket;
    const unsigned int wrongPin = pin + 1;
    const PinConfigInfoPacket::PinDirection wrongDirection = (
            (direction == PinConfigInfoPacket::DIR_OUTPUT) ?
                PinConfigInfoPacket::DIR_INPUT :
                PinConfigInfoPacket::DIR_OUTPUT
            );

    // cycle 1: no response

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    CHECK(NULL != requestPacket);
    CHECK(NULL != dynamic_cast<PinConfigPacket*>(requestPacket));

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    POINTERS_EQUAL(NULL, requestPacket);

    // cycle 2: no response

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    CHECK(NULL != requestPacket);
    CHECK(NULL != dynamic_cast<PinConfigPacket*>(requestPacket));

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    POINTERS_EQUAL(NULL, requestPacket);

    // cycle 3: bad responses

    PinConfigInfoPacket configInfoPacket1(wrongPin, direction);

    CHECK_FALSE(component.processPacket(configInfoPacket1));

    PinConfigInfoPacket configInfoPacket2(pin, wrongDirection);

    CHECK(component.processPacket(configInfoPacket2));

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    CHECK(NULL != requestPacket);
    CHECK(NULL != dynamic_cast<PinConfigPacket*>(requestPacket));

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    POINTERS_EQUAL(NULL, requestPacket);

    // cycle 4: good response

    PinConfigInfoPacket configInfoPacket3(pin, direction);

    CHECK(component.processPacket(configInfoPacket3));

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    CHECK(NULL != requestPacket);
    CHECK(NULL != dynamic_cast<RequestType*>(requestPacket));

    requestPacket = component.getNextPacket();
    packets.push_back(requestPacket);

    POINTERS_EQUAL(NULL, requestPacket);
}
