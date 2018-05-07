
#ifndef ENCODER_H
#define ENCODER_H

#include "RedBotPacket.h"

namespace frc
{
  class CounterBase
  {
  public:
    virtual ~CounterBase(){}
  };
}; /* namespace frc */

class EncoderInputPacket : public RedBotPacket
{
 public:

  EncoderInputPacket();

  /**
   * Constructor given which motor encoder is attached to
   */
  EncoderInputPacket(bool isRight);

  bool isRight() const;

  void read(std::istream& inputStream);

  bool isValid() const;

  bool operator==(const Packet& packet) const;

 private:

  void writeContents(std::ostream& outputStream) const;

  void getXMLElements(XMLElements& elements) const;

  bool myIsRight;
};

class EncoderCountPacket : public RedBotPacket
{
 public:

  EncoderCountPacket();

  EncoderCountPacket(bool isRight, int32_t count);

  bool isRight() const;

  int32_t getCount() const;

  void read(std::istream& inputStream);

  bool isValid() const;

  bool operator==(const Packet& packet) const;

 private:

  void writeContents(std::ostream& outputStream) const;

  void getXMLElements(XMLElements& elements) const;

  bool myIsRight;

  int32_t myCount;
};

class EncoderClearPacket : public RedBotPacket
{
 public:

  EncoderClearPacket();

  EncoderClearPacket(bool isRight);

  bool isRight() const;

  void read(std::istream& inputStream);

  bool isValid() const;

  bool operator==(const Packet& packet) const;

 private:

  void writeContents(std::ostream& outputStream) const;

  void getXMLElements(XMLElements& elements) const;

  bool myIsRight;
};

class RedBotEncoder : public frc::CounterBase
{
 public:
};

#endif /* ifndef ENCODER_H */
