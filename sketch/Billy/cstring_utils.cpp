/*************** FILE DESCRIPTION ***************/

/**
 * Filename: cstring_utils.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A small collection of macros and functions for processing
 *           C-style (null-terminated) strings.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "cstring_utils.h"

// Relevant standard libraries.
#include <cstring>


/******************* FUNCTIONS ******************/

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

int32_t cstring_utils::to_lowercase(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    constexpr int32_t ascii_code_diff = 'a' - 'A';
    int32_t i = 0;
    for (size_t j = 0; str[j] != '\0'; ++j) {
        if (str[j] >= 'A' && str[j] <= 'Z') {
            str[j] += ascii_code_diff;
            ++i;
        }
    }

    return i;
}

int32_t cstring_utils::to_lowercase_until_char(char *str, char stopper)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    constexpr int32_t ascii_code_diff = 'a' - 'A';
    int32_t i = 0;
    for (size_t j = 0; str[j] != '\0' && str[j] != stopper; ++j) {
        if (str[j] >= 'A' && str[j] <= 'Z') {
            str[j] += ascii_code_diff;
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

    constexpr int32_t ascii_code_diff = 'a' - 'A';
    int32_t i = 0;
    for (size_t j = 0; str[j] != '\0'; ++j) {
        if (str[j] >= 'a' && str[j] <= 'z') {
            str[j] -= ascii_code_diff;
            ++i;
        }
    }

    return i;
}

int32_t cstring_utils::to_uppercase_until_char(char *str, char stopper)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    constexpr int32_t ascii_code_diff = 'a' - 'A';
    int32_t i = 0;
    for (size_t j = 0; str[j] != '\0' && str[j] != stopper; ++j) {
        if (str[j] >= 'a' && str[j] <= 'z') {
            str[j] -= ascii_code_diff;
            ++i;
        }
    }

    return i;
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

int32_t cstring_utils::trim_leading_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    size_t len = strlen(str);
    if (len == 0) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    size_t first = 0;
    while (first < len && (str[first] == '\r' || str[first] == '\n')) {
        ++first;
    }

    if (first == 0) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    if (first == len) {
        str[0] = '\0';

        return CSTRING_UTILS_PROCESSED;
    }

    size_t src = first;
    size_t dest = 0;
    while (src < len) {
        str[dest++] = str[src++];
    }

    str[dest] = '\0';

    return CSTRING_UTILS_PROCESSED;
}

int32_t cstring_utils::inner_cr_and_lf_groups_to_single_spaces(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    size_t len = strlen(str);
    if (len == 0) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    size_t first = 0;
    while (first < len && (str[first] == '\r' || str[first] == '\n')) {
        ++first;
    }

    size_t last = len - 1;
    while (last > first && (str[last] == '\r' || str[last] == '\n')) {
        --last;
    }

    if (last == first && (str[last] == '\r' || str[last] == '\n')) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    int32_t groups = 0;

    size_t src  = first;
    size_t dest = first;
    bool in_group = false;
    while (src <= last) {
        char c = str[src];
        if (c == '\r' || c == '\n') {
            if (!in_group) {
                in_group = true;
                str[dest++] = ' ';
                ++groups;
            }
            ++src;
        } else {
            in_group = false;
            str[dest++] = str[src++];
        }
    }

    size_t trailing = last + 1;
    while (trailing < len) {
        str[dest++] = str[trailing++];
    }

    str[dest] = '\0';

    return groups;
}

int32_t cstring_utils::count_trailing_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    size_t len = strlen(str);
    int32_t i = 0;
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

int32_t cstring_utils::to_single_line(char *str)
{
    bool modified = false;

    int32_t retval = cstring_utils::trim_leading_crs_and_lfs(str);
    if (retval < 0) {
        return CSTRING_UTILS_MEM_ERR;
    }
    modified |= (retval > 0);

    retval = cstring_utils::inner_cr_and_lf_groups_to_single_spaces(str);
    if (retval < 0) {
        return CSTRING_UTILS_MEM_ERR;
    }
    modified |= (retval > 0);

    retval = cstring_utils::cut_off_trailing_crs_and_lfs(str);
    if (retval < 0) {
        return CSTRING_UTILS_MEM_ERR;
    }
    modified |= (retval > 0);

    if (modified) {
        return CSTRING_UTILS_PROCESSED;
    } else {
        return CSTRING_UTILS_NOT_PROCESSED;
    }
}
