#ifndef REDBOTCOMPONENT_H
#define REDBOTCOMPONENT_H

#include "Component.h"

/**
 * Component that automatically registers itself
 *
 * This abstract base class only implements a default constructor that
 * registers each constructed component instance for use with a robot.
 */
class RedBotComponent : public Component
{
    public:

        /**
         * Default constructor
         *
         * This registers this component with a robot
         */
        RedBotComponent();

        /**
         * Destructor
         */
        virtual ~RedBotComponent(){}
};

#endif /* ifndef REDBOTCOMPONENT_H */
