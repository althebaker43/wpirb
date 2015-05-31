
#include "Joystick.h"


Joystick::Joystick(
        int port
        ) :
    mySDLStick(NULL)
{
    if (SDL_Init(SDL_INIT_JOYSTICK) != 0)
    {
        return;
    }

    if (SDL_NumJoysticks() <= 0)
    {
        return;
    }

    mySDLStick = SDL_JoystickOpen(0);
}

Joystick::~Joystick()
{
    if (mySDLStick != NULL)
    {
        SDL_JoystickClose(mySDLStick);
    }
}

bool
Joystick::isConnected() const
{
    return (
            (mySDLStick != NULL) &&
            (SDL_JoystickGetAttached(mySDLStick) == SDL_TRUE)
           );
}

float
Joystick::GetX(
        JoystickHand hand
        )
{
    if (mySDLStick == NULL)
    {
        return 0.0;
    }

    SDL_JoystickUpdate();

    float xVal = SDL_JoystickGetAxis(mySDLStick, 0);

    if (xVal >= 0)
    {
        return (xVal / MAX_AXIS);
    }
    else
    {
        return (-1 * (xVal / MIN_AXIS));
    }
}

float
Joystick::GetY(
        JoystickHand hand
        )
{
    if (mySDLStick == NULL)
    {
        return 0.0;
    }

    SDL_JoystickUpdate();

    float yVal = SDL_JoystickGetAxis(mySDLStick, 1);

    if (yVal >= 0)
    {
        return (-1 * (yVal / MAX_AXIS));
    }
    else
    {
        return (yVal / MIN_AXIS);
    }
}
