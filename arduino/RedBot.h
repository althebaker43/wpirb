#ifndef REDBOT_H
#define REDBOT_H

namespace rb
{

class RedBotMotors
{
    public:

        RedBotMotors();

        void rightMotor(
                int speed
                );

        void leftMotor(
                int speed
                );
};

enum WHEEL { LEFT, RIGHT, BOTH };

class RedBotEncoder
{
 public:

  RedBotEncoder(int leftPin, int rightPin);

  long getTicks(WHEEL wheel);

  void clearEnc(WHEEL wheel);
};

};

#endif /* ifndef REDBOT_H */
