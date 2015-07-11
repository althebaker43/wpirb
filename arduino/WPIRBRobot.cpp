
#include "WPIRBRobot.h"


WPIRBRobot::WPIRBRobot() :
    myPacketSize(0),
    myIsHeaderRead(false)
{
}

void
WPIRBRobot::setup()
{
    Serial.begin(9600);
}

void
WPIRBRobot::loop()
{
    if (Serial.available() > 0)
    {
        byte curByte = Serial.read();

        if (curByte == PACKET_BOUND)
        {
            if (myIsHeaderRead == false)
            {
                myPacketSize = 0;
                myPacketBuffer[myPacketSize++] = PACKET_BOUND;
                myIsHeaderRead = true;
            }
            else
            {
                if (myPacketSize < PACKET_BUFSIZE)
                {
                    if (myPacketSize > 1)
                    {
                        // Valid complete packet
                        myPacketBuffer[myPacketSize++] = PACKET_BOUND;
                        parsePacket();
                        myIsHeaderRead = false;
                    }
                    else
                    {
                        // Back-to-back packet bounds
                        myPacketSize = 0;
                        myPacketBuffer[myPacketSize++] = PACKET_BOUND;
                        myIsHeaderRead = true;
                    }
                }
                else
                {
                    // Packet too large
                    myIsHeaderRead = false;
                }
            }
        }
        else
        {
            if (myIsHeaderRead == true)
            {
                if (myPacketSize < PACKET_BUFSIZE)
                {
                    myPacketBuffer[myPacketSize++] = curByte;
                }
            }
        }
    }
}

void
WPIRBRobot::parsePacket()
{
  if (myPacketSize == 0 || myPacketSize > 10)
  {
    myPacketSize = 0;
    return;
  }
  
  if (myPacketBuffer[0] != PACKET_BOUND)
  {
    return;
  }
  
  if (myPacketBuffer[myPacketSize - 1] != PACKET_BOUND)
  {
    return;
  }
  
  switch (myPacketBuffer[1])
  {
    case PACKET_TYPE_PING:
      parsePingPacket();
      break;
      
    case PACKET_TYPE_DOUTPUT:
      parseDigitalOutputPacket();
      break;
      
    case PACKET_TYPE_DINPUT:
      parseDigitalInputPacket();
      break;

    case PACKET_TYPE_PINCONFIG:
      parsePinConfigPacket();
      break;

    case PACKET_TYPE_MDRIVE:
      parseMotorDrivePacket();
      break;
      
    default:
      acknowledge();
      break;
  };
}

void
WPIRBRobot::parsePingPacket()
{
  acknowledge();
}

void
WPIRBRobot::parseDigitalOutputPacket()
{
  if (myPacketSize == 5)
  {
    unsigned int pin = myPacketBuffer[2];
    unsigned int value = (myPacketBuffer[3] == 0x02) ? HIGH : LOW;
    
    if (pin <= 13)
    {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, value);
    }
  }

  acknowledge();
  return;
}

void
WPIRBRobot::parseDigitalInputPacket()
{
  if (myPacketSize == 4)
  {
    unsigned int pin = myPacketBuffer[2];
    
    if (pin < 13)
    {
      pinMode(pin, INPUT);
      boolean value = digitalRead(pin);
      sendDigitalValue(pin, value);
    }
    else
    {
      acknowledge();
    }
  }
  else
  {
    acknowledge();
  }
  
  return;
}

void
WPIRBRobot::parsePinConfigPacket()
{
    if (myPacketSize == 5)
    {
        unsigned int pin = myPacketBuffer[2];
        unsigned int direction = myPacketBuffer[3];

        if (pin < 13)
        {
            pinMode(
                    pin,
                    ((direction == 1) ? OUTPUT : INPUT)
                   );
        }
    }

    acknowledge();
    return;
}

void
WPIRBRobot::parseMotorDrivePacket()
{
    if (myPacketSize == 7)
    {
        byte motor = myPacketBuffer[2];
        byte direction = myPacketBuffer[3];
        byte speedHi = myPacketBuffer[4];
        byte speedLo = myPacketBuffer[5];

        unsigned int speed = ((speedHi - 1) << 4) | (speedLo - 1);

        // Do not attempt to drive at very low speeds
        if (speed <= MOTOR_SPEED_THRESHOLD)
        {
            speed = 0;
        }

        switch (direction)
        {
            case 0x01:
                switch (motor)
                {
                    case 0x01:
                        myMotors.rightMotor(speed);
                        break;
                        
                    case 0x02:
                        myMotors.leftMotor(-1 * speed);
                        break;

                    default:
                        break;
                };
                break;

            case 0x02:
                switch (motor)
                {
                    case 0x01:
                        myMotors.rightMotor(-1 * speed);
                        break;
                        
                    case 0x02:
                        myMotors.leftMotor(speed);
                        break;

                    default:
                        break;
                };
                break;

            default:
                break;
        };
    }

    acknowledge();
    return;
}

void
WPIRBRobot::acknowledge()
{
  Serial.write(PACKET_BOUND);
  Serial.write(PACKET_TYPE_ACK);
  Serial.write(PACKET_BOUND);
  
  Serial.flush();
}

void
WPIRBRobot::sendDigitalValue(unsigned int pin, boolean value)
{
  Serial.write(PACKET_BOUND);
  Serial.write(PACKET_TYPE_DVALUE);
  Serial.write(byte(pin));
  Serial.write((value == true) ? 0x02 : 0x01);
  Serial.write(PACKET_BOUND);
  
  Serial.flush();
}
