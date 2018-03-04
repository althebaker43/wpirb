
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

    std::cout << "Found " << joystick.GetButtonCount() << " buttons." << std::endl;
    for (size_t buttonIdx = 0; buttonIdx < joystick.GetButtonCount(); ++buttonIdx)
      {
	std::cout << "Push joystick button " << buttonIdx << std::endl;
	while (joystick.GetRawButton(buttonIdx) == false);
      }

    std::cout << "Info: joystick test successful." << std::endl;

    return 0;
}
