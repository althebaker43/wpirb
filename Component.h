#ifndef COMPONENT_H
#define COMPONENT_H

// Forward declarations
class Packet;

/**
 * Interface for all robot components usable in programs
 */
class Component
{
    public:

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
};

#endif /* ifndef COMPONENT_H */
