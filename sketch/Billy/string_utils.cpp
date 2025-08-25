/*************** FILE DESCRIPTION ***************/

/**
 * Filename: string_utils.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  General-purpose macros and functions for use with
 *           null-terminated (C-style) strings and similar data.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Relevant standard libraries
#include <cstdint>
#include <cstring>

// Local modules
#include "string_utils.h"


/******************* FUNCTIONS ******************/

int32_t string_utils::to_lowercase_string(char *buf)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (uint32_t j = 0, len = strlen(buf); j < len; ++j) {
        if (buf[j] >= 'A' && buf[j] <= 'Z') {
            buf[j] += 'a' - 'A';  // Difference between an ASCII code of an uppercase and a lowercase letter.
            ++i;
        }
    }

    return i;
}

int32_t string_utils::to_uppercase_string(char *buf)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (uint32_t j = 0, len = strlen(buf); j < len; ++j) {
        if (buf[j] >= 'a' && buf[j] <= 'z') {
            buf[j] -= 'a' - 'A';  // Difference between an ASCII code of an uppercase and a lowercase letter.
            ++i;
        }
    }

    return i;
}


int32_t string_utils::nullify_first_cr_or_lf_in_string(char *buf)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }
    
    for (uint32_t i = 0, len = strlen(buf); i < len; ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';

            return STRING_UTILS_TRIGGERED;
        }
    }

    return STRING_UTILS_NOT_TRIGGERED;
}

int32_t string_utils::nullify_all_trailing_cr_and_lf_in_string(char *buf)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    uint32_t len = strlen(buf);
    int32_t i = 0;
    while (buf[len - 1] == '\r' || buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
        --len;
        ++i;
    }

    return i;
}

int32_t string_utils::append_cr_to_string(char *buf, size_t buf_size)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(buf);
    if (buf_size - len >= 2) {  // One byte for CR, another byte for null.
        buf[len] = '\r';
        buf[len + 1] = '\0';
 
        return STRING_UTILS_TRIGGERED;
    } else {
        return STRING_UTILS_NOT_TRIGGERED;
    }
}

int32_t string_utils::append_lf_to_string(char *buf, size_t buf_size)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(buf);
    if (buf_size - len >= 2) {  // One byte for LF, another byte for null.
        buf[len] = '\n';
        buf[len + 1] = '\0';

        return STRING_UTILS_TRIGGERED;
    } else {
        return STRING_UTILS_NOT_TRIGGERED;
    }
}

int32_t string_utils::append_lf_to_string_if_no_trailing(char *buf, size_t buf_size)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(buf);
    if (buf[len - 1] == '\n') {
        return STRING_UTILS_NOT_TRIGGERED;
    } else if (buf_size - len >= 2) {  // One byte for LF, another byte for a null.
        buf[len] = '\n';
        buf[len + 1] = '\0';

        return STRING_UTILS_TRIGGERED;
    } else {
        return STRING_UTILS_MEM_ERR;
    }
}

int32_t string_utils::nullify_all_cr_and_lf_in_char_array(char *buf, size_t buf_size)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (uint32_t j = 0; j < buf_size; ++j) {
        if (buf[j] == '\r' || buf[j] == '\n') {
            buf[j] = '\0';
            ++i;
        }
    }

    return i;
}

int32_t string_utils::substitute_all_cr_and_lf_in_char_array(char *buf, size_t buf_size, char character)
{
    if (!buf) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (uint32_t j = 0; j < buf_size - 1; ++j) {
        if (buf[j] == '\r' || buf[j] == '\n') {
            buf[j] = character;
            ++i;
        }
    }

    return i;
}
