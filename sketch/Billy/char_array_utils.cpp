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

int32_t string_utils::to_lowercase(char *str)
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

int32_t string_utils::to_uppercase(char *str)
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

int32_t string_utils::nullify_first_cr_or_lf(char *str)
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

int32_t string_utils::nullify_trailing_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (len == 0) {
        return STRING_UTILS_NOT_PROCESSED;
    }
    
    int32_t i = 0;
    while (str[len - 1] == '\r' || str[len - 1] == '\n') {
        str[len - 1] = '\0';
        --len;
        ++i;
    }

    return i;
}

int32_t string_utils::nullify_all_crs_and_lfs(char *str)
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

int32_t string_utils::append_cr(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for CR, another byte for null.
        return STRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\r';
    str[len + 1] = '\0';
 
    return STRING_UTILS_PROCESSED;
}

int32_t string_utils::append_lf(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for CR, another byte for null.
        return STRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\n';
    str[len + 1] = '\0';
 
    return STRING_UTILS_PROCESSED;
}

int32_t string_utils::append_char(char *str, size_t arr_size, char char_to_append)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for CR, another byte for null.
        return STRING_UTILS_MEM_ERR;
    }
  
    str[len] = char_to_append;
    str[len + 1] = '\0';
 
    return STRING_UTILS_PROCESSED;
}

int32_t string_utils::append_lf_if_no_trailing(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return STRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for CR, another byte for null.
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
