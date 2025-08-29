/*************** FILE DESCRIPTION ***************/

/**
 * Filename: char_array_utils.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  General-purpose macros and functions for use with
 *           null-terminated (C-style) strings and plain char arrays.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Source file's own header.
#include "char_array_utils.h"

// Relevant standard libraries.
#include <cstring>


/******************* FUNCTIONS ******************/

int32_t char_array_utils::cstring::to_lowercase(char *str)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (int32_t j = 0, len = strlen(str); j < len; ++j) {
        if (str[j] >= 'A' && str[j] <= 'Z') {
            str[j] += 'a' - 'A';  // Difference between an ASCII code of an uppercase and a lowercase letter.
            ++i;
        }
    }

    return i;
}

int32_t char_array_utils::cstring::to_uppercase(char *str)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (int32_t j = 0, len = strlen(str); j < len; ++j) {
        if (str[j] >= 'a' && str[j] <= 'z') {
            str[j] -= 'a' - 'A';  // Difference between an ASCII code of an uppercase and a lowercase letter.
            ++i;
        }
    }

    return i;
}

int32_t char_array_utils::cstring::nullify_first_cr_or_lf(char *str)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }
    
    for (int32_t i = 0, len = strlen(str); i < len; ++i) {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';

            return STRING_UTILS_PROCESSED;
        }
    }

    return STRING_UTILS_NOT_PROCESSED;
}

int32_t char_array_utils::cstring::nullify_all_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (int32_t j = 0, len = strlen(str); j < len; ++j) {
        if (str[j] == '\r' || str[j] == '\n') {
            str[j] = '\0';
            ++i;
        }
    }

    return i;
}

int32_t char_array_utils::cstring::count_trailing_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (len == 0) {
        return STRING_UTILS_NOT_PROCESSED;
    }

    int32_t i = 0;
    while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
        --len;
        ++i;
    }

    return i;
}

int32_t char_array_utils::cstring::nullify_trailing_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (len == 0) {
        return STRING_UTILS_NOT_PROCESSED;
    }

    int32_t trailing = char_array_utils::cstring::count_trailing_crs_and_lfs(str);
    
    int32_t i = 0;
    for (; i < trailing; ++i) {
        str[len - 1 - i] = '\0';
    }

    return i;
}

int32_t char_array_utils::cstring::append_cr(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return STRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\r';
    str[len + 1] = '\0';
 
    return STRING_UTILS_PROCESSED;
}

int32_t char_array_utils::cstring::append_lf(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return STRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\n';
    str[len + 1] = '\0';
 
    return STRING_UTILS_PROCESSED;
}

int32_t char_array_utils::cstring::append_char(char *str, size_t arr_size, char char_to_append)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return STRING_UTILS_MEM_ERR;
    }
  
    str[len] = char_to_append;
    str[len + 1] = '\0';
 
    return STRING_UTILS_PROCESSED;
}

int32_t char_array_utils::cstring::append_lf_if_no_trailing(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        --len;
        ++i;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return STRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\n';
    str[len + 1] = '\0';
 
    return STRING_UTILS_PROCESSED;






    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (!len) {
        return STRING_UTILS_NOT_PROCESSED;
    }
    
    if (str[len - 1] == '\n') {
        return STRING_UTILS_NOT_PROCESSED;
    } else if (arr_size - len >= 2) {  // One byte for LF, another byte for a null.
        str[len] = '\n';
        str[len + 1] = '\0';

        return STRING_UTILS_PROCESSED;
    } else {
        return STRING_UTILS_MEM_ERR;
    }
}

int32_t string_utils::char_array_utils::nullify_all_cr_and_lf_in_char_array(char *str,
                                                                            size_t arr_size)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (int32_t j = 0; j < arr_size; ++j) {
        if (str[j] == '\r' || str[j] == '\n') {
            str[j] = '\0';
            ++i;
        }
    }

    return i;
}

int32_t string_utils::char_array_utils::substitute_all_cr_and_lf_in_char_array(char *str,
                                                                               size_t arr_size,
                                                                               char character)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t i = 0;
    for (int32_t j = 0; j < arr_size; ++j) {
        if (str[j] == '\r' || str[j] == '\n') {
            str[j] = character;
            ++i;
        }
    }

    return i;
}
