#ifndef TESTWPIRBROBOT_H
#define TESTWPIRBROBOT_H

class WPIRBRobot;
class Packet;

void
SendPacket(
        const Packet&   requestPacket,
        const Packet&   responsePacket,
        WPIRBRobot&     robot
        );


#endif /* ifndef TESTWPIRBROBOT_H */
