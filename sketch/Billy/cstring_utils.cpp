/*************** FILE DESCRIPTION ***************/

/**
 * Filename: cstring_utils.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A handful of macros and functions for use with
 *           C-style (null-terminated) strings.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Source file's own header.
#include "cstring_utils.h"

// Relevant standard libraries.
#include <cstring>


/******************* FUNCTIONS ******************/

int32_t cstring_utils::to_lowercase(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (size_t j = 0, len = strlen(str); j < len; ++j) {
        if (str[j] >= 'A' && str[j] <= 'Z') {
            str[j] += 'a' - 'A';  // Difference between an ASCII code of an uppercase and a lowercase letter.
            ++i;
        }
    }

    return i;
}

int32_t cstring_utils::to_uppercase(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (size_t j = 0, len = strlen(str); j < len; ++j) {
        if (str[j] >= 'a' && str[j] <= 'z') {
            str[j] -= 'a' - 'A';  // Difference between an ASCII code of an uppercase and a lowercase letter.
            ++i;
        }
    }

    return i;
}

int32_t cstring_utils::to_single_line(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    size_t len = strlen(str);
    if (len == 0) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    size_t start = 0;
    while (start < len && (str[start] == '\r' || str[start] == '\n')) {
        ++start;
    }

    size_t last = len - 1;
    while (last > start && (str[last] == '\r' || str[last] == '\n')) {
        --last;
    }

    if (last == start && (str[last] == '\r' || str[last] == '\n')) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    int32_t groups = 0;

    size_t src  = start;
    size_t dest = start;
    bool in_group = false;
    while (src <= last) {
        char c = str[src];
        if (c == '\r' || c == '\n') {
            if (!in_group) {
                str[dest++] = ' ';
                ++groups;
                in_group = true;
            }
            ++src;
        } else {
            in_group = false;
            str[dest++] = str[src++];
        }
    }

    size_t trailing_begin = last + 1;
    while (trailing_begin < len) {
        str[dest++] = str[trailing_begin++];
    }

    str[dest] = '\0';

    return groups;
}

int32_t cstring_utils::append_char(char *str, size_t arr_size, char char_to_append)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    size_t len = strlen(str);
    if (arr_size < len + 2) {  // One byte for an appended character, another byte for null.
        return CSTRING_UTILS_MEM_ERR;
    }

    str[len] = char_to_append;
    str[len + 1] = '\0';

    return CSTRING_UTILS_PROCESSED;
}

int32_t cstring_utils::nullify_first_cr_or_lf(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    for (size_t i = 0, len = strlen(str); i < len; ++i) {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';

            return CSTRING_UTILS_PROCESSED;
        }
    }

    return CSTRING_UTILS_NOT_PROCESSED;
}

int32_t cstring_utils::count_trailing_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    size_t len = strlen(str);
    int32_t i = 0;
    /* len can decrement during the loop execution,
     * therefore it is checked to prevent an underflow.
     */ 
    while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
        --len;
        ++i;
    }

    return i;
}

int32_t cstring_utils::cut_off_trailing_crs_and_lfs(char *str)
{
    int32_t count = cstring_utils::count_trailing_crs_and_lfs(str);

    if (count < 0) {
        return CSTRING_UTILS_MEM_ERR;
    } else if (count == 0) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    size_t len = strlen(str);
    str[len - (size_t)count] = '\0';

    return CSTRING_UTILS_PROCESSED;
}
