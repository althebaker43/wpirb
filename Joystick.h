#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "TwoDimController.h"
#include "SDL2/SDL.h"

namespace frc
{

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
class Joystick : public TwoDimController
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

        // TwoDimController interface

        /**
         * Provides this controller's position on the X axis
         */
        float getXPosition();

        /**
         * Provides this controller's position on the Y axis
         */
        float getYPosition();

	/**
	 * Provides the number of controller buttons
	 */
	int GetButtonCount() const;

	/**
	 * Provides the state of the button at the given index
	 */
	bool GetRawButton(int) const;

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

}; /* namespace frc */

#endif /* ifndef JOYSTICK_H */
