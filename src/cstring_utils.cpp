/*************** FILE DESCRIPTION ***************/

/**
 * Filename: cstring_utils.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A collection of macros and functions for inspecting and processing
 *           C-style (null-terminated) strings.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "src/cstring_utils.h"

// Relevant standard libraries.
#include <cstring>


/******************* FUNCTIONS ******************/

/*--- Inspection functions ---*/

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

bool cstring_utils::are_equal(const char *str1, const char *str2)
{
    if (str1 == nullptr || str2 == nullptr) {
        return false;
    }

    if (strcmp(str1, str2) == 0) {
        return true;
    } else {
        return false;
    }
}

bool cstring_utils::are_equal_within_bytes(const char *str1, const char *str2, size_t bytes_to_compare)
{
    if (str1 == nullptr || str2 == nullptr) {
        return false;
    }

    if (strncmp(str1, str2, bytes_to_compare) == 0) {
        return true;
    } else {
        return false;
    }
}

int32_t cstring_utils::count_leading_crs_and_lfs(const char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_ERR_MEM;
    }

    int32_t i = 0;
    while (str[i] == '\r' || str[i] == '\n') {
        ++i;
    }

    return i;
}

int32_t cstring_utils::count_trailing_crs_and_lfs(const char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_ERR_MEM;
    }

    int32_t i = 0;
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
        --len;
        ++i;
    }

    return i;
}


/*--- Processing functions ---*/

int32_t cstring_utils::append_char(char *str, size_t arr_size, char char_to_append)
{
    if (str == nullptr || !is_string(str, arr_size)) {
        return CSTRING_UTILS_ERR_MEM;
    }

    size_t len = strlen(str);
    if (arr_size < len + 2) {  // One byte for a character to be appended, another byte for null.
        return CSTRING_UTILS_ERR_MEM;
    }

    str[len] = char_to_append;
    str[len + 1] = '\0';

    return CSTRING_UTILS_PROCESSED;
}

int32_t cstring_utils::to_lowercase(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_ERR_MEM;
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

int32_t cstring_utils::to_lowercase_before_char(char *str, char stopper)
{
    if (str == nullptr) {
        return CSTRING_UTILS_ERR_MEM;
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
        return CSTRING_UTILS_ERR_MEM;
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

int32_t cstring_utils::to_uppercase_before_char(char *str, char stopper)
{
    if (str == nullptr) {
        return CSTRING_UTILS_ERR_MEM;
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
        return CSTRING_UTILS_ERR_MEM;
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
    int32_t leading = count_leading_crs_and_lfs(str);

    if (leading < 0) {
        return CSTRING_UTILS_ERR_MEM;
    } else if (leading == 0) {
        // A string lacking leading CRs and/or LFs should not be processed.
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    // A string consisting of CRs and/or LFs only should not be processed.
    size_t len = strlen(str);
    if (static_cast<size_t>(leading) == len) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    char *after_leading = str + leading;
    size_t bytes_to_move = len - static_cast<size_t>(leading) + 1;  /* A single byte is added
                                                                     * to include a null terminator.
                                                                     */
    memmove(str, after_leading, bytes_to_move);

    return static_cast<int32_t>(leading);
}

int32_t cstring_utils::inner_cr_and_lf_groups_to_single_spaces(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_ERR_MEM;
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

    // A string consisting of CRs and/or LFs only should not be processed.
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

int32_t cstring_utils::cut_off_trailing_crs_and_lfs(char *str)
{
    int32_t trailing = cstring_utils::count_trailing_crs_and_lfs(str);

    if (trailing < 0) {
        return CSTRING_UTILS_ERR_MEM;
    } else if (trailing == 0) {
        // A string lacking trailing CRs and/or LFs should not be processed.
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    // A string consisting of CRs and/or LFs only should not be processed.
    size_t len = strlen(str);
    if (static_cast<size_t>(trailing) == len) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    str[len - static_cast<size_t>(trailing)] = '\0';

    return static_cast<int32_t>(trailing);
}

int32_t cstring_utils::to_single_line(char *str)
{
    int32_t modified = 0;

    int32_t retval = cstring_utils::trim_leading_crs_and_lfs(str);
    if (retval < 0) {
        return CSTRING_UTILS_ERR_MEM;
    } else if (retval > 0) {
        ++modified;
    }

    retval = cstring_utils::inner_cr_and_lf_groups_to_single_spaces(str);
    if (retval < 0) {
        return CSTRING_UTILS_ERR_MEM;
    } else if (retval > 0) {
        ++modified;
    }

    retval = cstring_utils::cut_off_trailing_crs_and_lfs(str);
    if (retval < 0) {
        return CSTRING_UTILS_ERR_MEM;
    } else if (retval > 0) {
        ++modified;
    }

    return modified;
}
