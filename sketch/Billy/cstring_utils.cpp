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

bool cstring_utils::is_string(const char *arr, size_t arr_size)
{
    if (arr == nullptr || arr_size == 0) {
        return false;
    }

    for (size_t i = 0; i < arr_size; ++i) {
        if (arr[i] == '\0') {
            return true;
        }
    }

    return false;
}

int32_t cstring_utils::append_char(char *str, size_t arr_size, char char_to_append)
{
    if (str == nullptr || !is_string(str, arr_size)) {
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

bool cstring_utils::are_equal(const char *str1, const char *str2)
{
    if (str1 == nullptr || str2 == nullptr) {
        return false;
    }

    if (!strcmp(str1, str2)) {
        return true;
    } else {
        return false;
    }
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

    for (size_t i = 0; str[i] != '\0'; ++i) {
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

    size_t leading = 0;
    while (str[leading] == '\r' || str[leading] == '\n') {
        ++leading;
    }

    if (leading == 0 || leading == len) {
        return CSTRING_UTILS_NOT_PROCESSED;    /* A string lacking leading CRs and/or LFs,
                                                * as well as a string consisting of CRs and/or LFs only,
                                                * should not be processed.
                                                */
    }

    char *after_leading = str + leading;
    size_t bytes_to_move = len - leading + 1;  // A single byte is added to include a null terminator.
    memmove(str, after_leading, bytes_to_move);

    return (int32_t)leading;
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

int32_t cstring_utils::count_trailing_crs_and_lfs(const char *str)
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
    int32_t modified = 0;

    int32_t retval = cstring_utils::trim_leading_crs_and_lfs(str);
    if (retval < 0) {
        return CSTRING_UTILS_MEM_ERR;
    } else if (retval > 0) {
        ++modified;
    }

    retval = cstring_utils::inner_cr_and_lf_groups_to_single_spaces(str);
    if (retval < 0) {
        return CSTRING_UTILS_MEM_ERR;
    } else if (retval > 0) {
        ++modified;
    }

    retval = cstring_utils::cut_off_trailing_crs_and_lfs(str);
    if (retval < 0) {
        return CSTRING_UTILS_MEM_ERR;
    } else if (retval > 0) {
        ++modified;
    }

    return modified;
}
