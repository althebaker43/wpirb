
#include "RedBotEncoder.h"


EncoderInputPacket::EncoderInputPacket() :
  RedBotPacket(TYPE_ENCINPUT, "ENCINPUT", BID_ENCINPUT),
  myIsRight(true)
{
}

EncoderInputPacket::EncoderInputPacket(bool isRight) :
  RedBotPacket(TYPE_ENCINPUT, "ENCINPUT", BID_ENCINPUT),
  myIsRight(isRight)
{
}

bool
EncoderInputPacket::isRight() const
{
  return myIsRight;
}

void
EncoderInputPacket::writeContents(std::ostream& outputStream) const
{
  outputStream << (myIsRight ? '\x02' : '\x01');
}

void
EncoderInputPacket::getXMLElements(XMLElements& elements) const
{
  elements.add(new XMLDataElement<const char*>("motor", (myIsRight ? "right" : "left")));
}

void
EncoderInputPacket::read(std::istream& inputStream)
{
  bool isRight = true;
  isRight = (inputStream.get() == 2);
  if (inputStream.good() == false)
    {
      return;
    }

  myIsRight = isRight;

  inputStream.get();
}

bool
EncoderInputPacket::isValid() const
{
  return true;
}

bool
EncoderInputPacket::operator==(const Packet& packet) const
{
  return true;
}


EncoderCountPacket::EncoderCountPacket() :
  RedBotPacket(TYPE_ENCCOUNT, "ENCCOUNT", BID_ENCCOUNT),
  myIsRight(true),
  myCount(0)
{
}

EncoderCountPacket::EncoderCountPacket(bool isRight, int32_t count) :
  RedBotPacket(TYPE_ENCCOUNT, "ENCCOUNT", BID_ENCCOUNT),
  myIsRight(isRight),
  myCount(count)
{
}

bool
EncoderCountPacket::isRight() const
{
  return myIsRight;
}

int32_t
EncoderCountPacket::getCount() const
{
  return myCount;
}

void
EncoderCountPacket::read(std::istream& inputStream)
{
  bool isRight = true;
  isRight = (inputStream.get() == 1);
  if (inputStream.good() == false)
    {
      return;
    }

  myIsRight = isRight;

  int32_t count = 0;
  for (ssize_t upperByteIdx = 32; upperByteIdx >= 0; upperByteIdx -= 7)
    {
      char curByte = inputStream.get();
      if (inputStream.good() == false)
	{
	  return;
	}

      --curByte;

      ssize_t shiftAmt = (upperByteIdx > 7) ? upperByteIdx - 7 : 0;
      count |= (curByte << shiftAmt);
    }

  myCount = count;

  inputStream.get();
}

bool
EncoderCountPacket::isValid() const
{
  return true;
}

bool
EncoderCountPacket::operator==(const Packet& packet) const
{
  return true;
}

void
EncoderCountPacket::writeContents(std::ostream& outputStream) const
{
  outputStream << (myIsRight ? "\x01" : "\x02");

  // Split count up into 7-bit chunks
  for (ssize_t bitOffset = 31; bitOffset >= 0; bitOffset -= 7)
    {
      char curByte = 0;
      for (ssize_t bitIdx = bitOffset; (bitIdx >= 0) && (bitIdx > bitOffset - 7); --bitIdx)
	{
	  curByte = (curByte << 1) | ((myCount & (1 << bitIdx)) ? 1 : 0);
	}
      ++curByte;

      outputStream << curByte;
    }
}

void
EncoderCountPacket::getXMLElements(XMLElements& elements) const
{
  elements.add(new XMLDataElement<const char*>("motor", (myIsRight ? "right" : "left")));
  elements.add(new XMLDataElement<int32_t>("count", myCount));
}


EncoderClearPacket::EncoderClearPacket() :
  RedBotPacket(TYPE_ENCCLEAR, "ENCCLEAR", BID_ENCCLEAR),
  myIsRight(true)
{
}

EncoderClearPacket::EncoderClearPacket(bool isRight) :
  RedBotPacket(TYPE_ENCCLEAR, "ENCCLEAR", BID_ENCCLEAR),
  myIsRight(isRight)
{
}

bool
EncoderClearPacket::isRight() const
{
  return myIsRight;
}

void
EncoderClearPacket::read(std::istream& inputStream)
{
  bool isRight = true;
  isRight = (inputStream.get() == 1);
  if (inputStream.good() == false)
    {
      return;
    }

  myIsRight = isRight;

  inputStream.get();
}

bool
EncoderClearPacket::isValid() const
{
  return true;
}

bool
EncoderClearPacket::operator==(const Packet& packet) const
{
  return true;
}

void
EncoderClearPacket::writeContents(std::ostream& outputStream) const
{
  outputStream << (myIsRight ? '\x01' : '\x02');
}

void
EncoderClearPacket::getXMLElements(XMLElements& elements) const
{
  elements.add(new XMLDataElement<const char*>("motor", (myIsRight ? "right" : "left")));
}
