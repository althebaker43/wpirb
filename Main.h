#ifndef MAIN_H
#define MAIN_H

namespace frc
{
  class IterativeRobot;
};

/**
 * WPIRB program entry point
 */
int
WPIRBMain(
        int             argc,
        char**          argv,
        frc::IterativeRobot& program /**< Robot program to run */
        );

#endif /* ifndef MAIN_H */
