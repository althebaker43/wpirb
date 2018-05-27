#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdint.h>

typedef bool boolean;
typedef uint8_t byte;

enum Value
{
    LOW =   0,
    HIGH =  1
};

enum Mode
{
    OUTPUT,
    INPUT
};

enum AnalogInputs
  {
    A2
  };

class SerialHandler
{
    public:

        void begin(
                unsigned int baud
                );

        unsigned int available();

        byte read();

        void write(
                byte data
                );

        void flush();
};

extern SerialHandler Serial;

void
pinMode(
        unsigned int    pin,
        unsigned int    mode
       );

void
digitalWrite(
        unsigned int    pin,
        unsigned int    value
        );

unsigned int
digitalRead(
        unsigned int pin
        );

unsigned int
analogRead(
        unsigned int pin
        );

#endif /* ifndef ARDUINO_H */
