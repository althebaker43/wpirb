#ifndef MAIN_H
#define MAIN_H

class IterativeRobot;

/**
 * WPIRB program entry point
 */
int
WPIRBMain(
        int             argc,
        char**          argv,
        IterativeRobot& program /**< Robot program to run */
        );

#endif /* ifndef MAIN_H */
