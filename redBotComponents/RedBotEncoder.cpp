
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
