#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "SDL2/SDL.h"

/**
 * Enumeration for different joysticks
 */
enum JoystickHand
{
    kRightHand,
    kLeftHand
};

/**
 * Encapsulates a joystick or gamepad input device
 *
 * This class currently uses the Simple DirectMedia Layer (SDL) to use
 * connected joysticks.
 */
class Joystick
{
    public:

        /**
         * Initializes a joystick at the given port
         */
        Joystick(
                int port
                );

        /**
         * Destructor
         */
        ~Joystick();

        /**
         * Gets the position of the stick on the X-axis
         */
        float GetX(
                JoystickHand    hand = kRightHand
                );

        /**
         * Gets the position of the stick on the Y-axis
         */
        float GetY(
                JoystickHand    hand = kRightHand
                );

        /**
         * Indicates if this joystick is connected
         */
        bool isConnected() const;

    private:

        /**
         * Maximum value for joystick axis
         */
        const static int MAX_AXIS = 32767;

        /**
         * Minimum value for joystick axis
         */
        const static int MIN_AXIS = -32768;

        /**
         * Handle to SDL joystick object
         */
        SDL_Joystick* mySDLStick;
};

#endif /* ifndef JOYSTICK_H */
