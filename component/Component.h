#ifndef COMPONENT_H
#define COMPONENT_H

#include "Packet.h"
#include <list>

// Forward declarations
class Component;

/**
 * Component collection type
 */
typedef std::list<Component*> Components;

/**
 * Interface for all robot components usable in programs
 */
class Component
{
    public:

        /**
         * Registers a component used in a robot program
         *
         * This is done during program initialization so that packets can be
         * exchanged between the robot and the different components.
         */
        static void RegisterComponent(
                Component* component
                );

        /**
         * Provides the list of all registered components
         */
        static const Components& GetRegisteredComponents();

        /**
         * Clears the registered components awaiting ownership
         */
        static void ClearRegisteredComponents();

        /**
         * Destructor
         */
        virtual ~Component(){};

        /**
         * Provides a pointer to the next packet to send to the robot
         *
         * If the returned pointer is non-NULL, the caller is responsible for
         * deleting packet that it points to.
         *
         * \return Pointer to packet to send if available or NULL if none
         * available
         */
        virtual Packet* getNextPacket() = 0;

        /**
         * Examines a packet and processes it if desired
         *
         * \return True if the packet was processed, false otherwise
         */
        virtual bool processPacket(
                const Packet& packet
                ) = 0;

    private:

        /**
         * Components used in last-initialized robot program
         *
         * After the program has finished being constructed, the robot instance
         * that owns the program should grab all of the components in this
         * collection to use for packet communication and then clear it.
         */
        static Components ourCurrentComponents;
};

#endif /* ifndef COMPONENT_H */
