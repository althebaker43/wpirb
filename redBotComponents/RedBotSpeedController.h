
#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include <stdlib.h>
#include "RedBotComponent.h"
#include "RedBotPacket.h"

/**
 * Motor drive command class
 */
class MotorDrivePacket : public RedBotPacket
{
    public:

        /**
         * Motor enumeration
         */
        enum Motor
        {
            MOTOR_LEFT,
            MOTOR_RIGHT
        };

        /**
         * Motor direction enumeration
         */
        enum Direction
        {
            DIR_FORWARD,
            DIR_BACKWARD
        };

        /**
         * Default constructor
         */
        MotorDrivePacket();

        /**
         * Constructor given motor information
         */
        MotorDrivePacket(
                Motor       motor,      /**< Motor to drive */
                uint8_t     speed,      /**< Absolute speed to drive motor to */
                Direction   direction   /**< Direction to drive motor in */
                );

        /**
         * Constructor given motor and single floating-point drive value
         *
         * This constructor automatically translates the given value to a valid
         * speed and direction.
         */
        MotorDrivePacket(
                Motor   motor,      /**< Motor to drive */
                double  driveVal    /**< Signed floating-point drive value */
                );

        /**
         * Destructor
         */
        ~MotorDrivePacket();

        /**
         * Reads serialized binary data from input stream
         */
        void read(
                std::istream&
                );

        /**
         * Indicates if this packet is valid or not
         *
         * A Packet can be invalid if it tried to read from a malformed byte
         * stream.
         */
        bool isValid() const;

        /**
         * Equality operator
         */
        bool operator==(
                const Packet&
                ) const;

        /**
         * Indicates which motor to drive
         */
        Motor getMotor() const;

        /**
         * Provides absolute speed to drive motor to
         */
        uint8_t getSpeed() const;

        /**
         * Indicates which way the motor will be driven
         */
        Direction getDirection() const;

    private:

        /**
         * Writes binary packet contents to output stream
         */
        void writeContents(
                std::ostream&
                ) const;

        /**
         * Provides elements to include in the XML representation
         */
        void getXMLElements(
                XMLElements& elements
                ) const;

        /**
         * Motor to drive
         */
        Motor myMotor;

        /**
         * Speed to drive motor to
         */
        uint8_t mySpeed;

        /**
         * Direction to drive motor in
         */
        Direction myDirection;
};

namespace frc
{

  class SpeedController
  {
  public:

    virtual ~SpeedController(){}

    virtual void Set(double speed) = 0;
  };

  class PWMSpeedController : public SpeedController
  {
  public:

    virtual ~PWMSpeedController(){}
  };

}; /* namespace frc */

class RedBotSpeedController : public frc::PWMSpeedController, public RedBotComponent
{
 public:

  RedBotSpeedController(size_t channel);

  void Set(double speed);

  Packet* getNextPacket();

  bool processPacket(const Packet& packet);

 private:

  const size_t myChannel;

  MotorDrivePacket* myCurrentMotorPacket;
};

#endif /* ifndef SPEEDCONTROLLER_H */
