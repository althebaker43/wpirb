
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
                    myPacketBuffer[myPacketSize++] = PACKET_BOUND;
                    parsePacket();
                }
                myIsHeaderRead = false;
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
    
    if (pin < 13)
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
