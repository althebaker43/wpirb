#ifndef TESTUTILS_H
#define TESTUTILS_H

#include "CppUTest/TestHarness.h"
#include <string.h>


/**
 * Explains the difference between two binary packets
 */
class PacketDiffExplainer
{
    public:

        /**
         * Constructor given two binary packet strings
         */
        PacketDiffExplainer(
                const char* expectedString,
                const char* actualString
                );

        /**
         * Provides a readable message explaining the difference
         */
        const char* getMessage() const;

    private:

        /**
         * Storage for error message
         */
        std::string myMessage;
};

#define BPACKET_EQUAL(expectedString, actualString) \
    CHECK_TEXT( \
            (strcmp(expectedString, actualString) == 0), \
            PacketDiffExplainer(expectedString, actualString).getMessage() \
            )

#endif /* ifndef TESTUTILS_H */
