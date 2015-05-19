#ifndef WPIRBROBOT_H
#define WPIRBROBOT_H

#include "Arduino.h"

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

        void acknowledge();
        void sendDigitalValue(
                unsigned int    pin,
                boolean         value
                );

        const static byte PACKET_BOUND = 0xFF;

        const static byte PACKET_TYPE_PING =    0x01;
        const static byte PACKET_TYPE_DOUTPUT = 0x02;
        const static byte PACKET_TYPE_DINPUT =  0x03;

        const static byte PACKET_TYPE_ACK =     0x82;
        const static byte PACKET_TYPE_DVALUE =  0x81;

        const static unsigned int PACKET_BUFSIZE = 10;

        byte myPacketBuffer[PACKET_BUFSIZE];

        unsigned int myPacketSize;

        boolean myIsHeaderRead;
};

#endif /* ifndef WPIRBROBOT_H */
