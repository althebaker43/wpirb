#ifndef WPIRBROBOT_H
#define WPIRBROBOT_H

#include "Arduino.h"
#include "RedBot.h"

class WPIRBRobot
{
    public:

        WPIRBRobot();

        void setup();

        void loop();

    private:

        void parsePacket();
        void parsePingPacket();
        void parseDigitalOutputPacket();
        void parseDigitalInputPacket();
        void parseAnalogInputPacket();
        void parsePinConfigPacket();
        void parseMotorDrivePacket();

        void acknowledge();
        void sendDigitalValue(
                unsigned int    pin,
                boolean         value
                );
        void sendAnalogValue(
                unsigned int    pin,
                unsigned int    value
                );

        const static byte PACKET_BOUND = 0xFF;

        const static byte PACKET_TYPE_PING =        0x01;
        const static byte PACKET_TYPE_DOUTPUT =     0x02;
        const static byte PACKET_TYPE_DINPUT =      0x03;
        const static byte PACKET_TYPE_AINPUT =      0x04;
        const static byte PACKET_TYPE_PINCONFIG =   0x05;
        const static byte PACKET_TYPE_MDRIVE =      0x06;

        const static byte PACKET_TYPE_ACK =     0x82;
        const static byte PACKET_TYPE_DVALUE =  0x81;
        const static byte PACKET_TYPE_AVALUE =  0x83;

        const static unsigned int PACKET_BUFSIZE = 10;

        const static unsigned int MOTOR_SPEED_THRESHOLD = 64;

        RedBotMotors myMotors;

        byte myPacketBuffer[PACKET_BUFSIZE];

        unsigned int myPacketSize;

        boolean myIsHeaderRead;
};

#endif /* ifndef WPIRBROBOT_H */
