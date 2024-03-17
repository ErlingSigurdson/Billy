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

/*--- String operations ---*/

bool utilities_nullify_first_CR_or_LF_in_string(char *buf)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';

            return 1;
        }
    }

    return 0;
}

bool utilities_nullify_all_CR_and_LF_in_char_array(char *buf, size_t buf_size)
{
    uint32_t i = 0;
    for (; i < buf_size - 1; ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';
        }
    }

    return i;
}

bool utilities_nullify_all_trailing_CR_and_LF_in_string(char *buf)
{
    uint32_t i = 0;
	while (buf[strlen(buf) - 1] == '\r' || buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = 0;
		++i;
	}

	return i;
}

bool utilities_substitute_all_CR_and_LF_in_char_array(char *buf, size_t buf_size, char character)
{
    uint32_t i = 0;
    for (; i < buf_size - 1; ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = character;
        }
    }

    return i;
}

bool utilities_append_LF_to_string(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // One byte for LF, another byte for null.
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_append_LF_if_absent_to_string(char *buf, size_t buf_size)
{
    if (buf[strlen(buf) - 1] == '\n') {
        return 0;
    } else if (buf_size - strlen(buf) >= 2) {  // One byte for LF, another byte for a null.
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_append_CR_to_string(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // One byte for CR, another byte for null.
        buf[strlen(buf)] = '\r';

        return 1;
    }

    return 0;
}
