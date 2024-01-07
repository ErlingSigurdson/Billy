/****************** DESCRIPTION *****************/

/**
 * Filename: utilities.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: some general purpose macros and functions.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>

// Local modules.
#include "utilities.h"


/******************* FUNCTIONS ******************/

bool utilities_remove_CR_and_LF(char *buf)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';

            return 1;
        }
    }

    return 0;
}

bool utilities_append_CR_and_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 3) {  // One byte for CR, another byte for LF, third byte for a null terminator.
        buf[strlen(buf)] = '\r';
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}

bool utilities_append_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // One byte for LF, another byte for a null terminator.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}

bool utilities_append_LF_if_absent(char *buf, size_t buf_size)
{
    if (strrchr(buf, '\n') == &buf[strlen(buf) - 1]) {
        return 0;
    } else if (buf_size - strlen(buf) >= 2) {  // One byte for LF, another byte for a null terminator.
        utilities_append_LF(buf, buf_size);

        return 1;
    }

    return 0;
}

bool utilities_force_2xLF(char *buf, size_t buf_size)
{
    utilities_remove_CR_and_LF(buf);
    if (buf_size - strlen(buf) >= 3) {  // Two bytes for LFs, another byte for a null terminator.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}
