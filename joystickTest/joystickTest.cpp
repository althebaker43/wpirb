
#include "WPILib.h"
#include <iostream>


int
main(
        int     argc,
        char*   argv[]
    )
{
    frc::Joystick joystick(0);

    if (joystick.isConnected() == false)
    {
        std::cerr << "Error: could not connect to joystick." << std::endl;
        return 1;
    }

    std::cout << "Info: Beginning joystick test." << std::endl;

    std::cout << "Move joystick up to continue." << std::endl;
    while (joystick.GetY() <= 0.9);

    std::cout << "Move joystick right to continue." << std::endl;
    while (joystick.GetX() <= 0.9);

    std::cout << "Move joystick down to continue." << std::endl;
    while (joystick.GetY() >= -0.9);

    std::cout << "Move joystick left to continue." << std::endl;
    while (joystick.GetX() >= -0.9);

    std::cout << "Info: joystick test successful." << std::endl;

    return 0;
}
