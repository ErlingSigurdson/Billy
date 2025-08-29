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
    for (int32_t j = 0, len = strlen(str); j < len; ++j) {
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
    for (int32_t j = 0, len = strlen(str); j < len; ++j) {
        if (str[j] >= 'a' && str[j] <= 'z') {
            str[j] -= 'a' - 'A';  // Difference between an ASCII code of an uppercase and a lowercase letter.
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
    
    for (int32_t i = 0, len = strlen(str); i < len; ++i) {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';

            return CSTRING_UTILS_PROCESSED;
        }
    }

    return CSTRING_UTILS_NOT_PROCESSED;
}

int32_t cstring_utils::nullify_trailing_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (len == 0) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    int32_t i = 0;
    while ((str[len - 1] == '\r' || str[len - 1] == '\n') && len > 0) {
        str[len - 1] = '\0';
        --len;
        ++i;
    }

    return i;
}

int32_t cstring_utils::nullify_all_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
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

int32_t cstring_utils::append_cr(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return CSTRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\r';
    str[len + 1] = '\0';
 
    return CSTRING_UTILS_PROCESSED;
}

int32_t cstring_utils::append_lf(char *str, size_t arr_size)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return CSTRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\n';
    str[len + 1] = '\0';
 
    return CSTRING_UTILS_PROCESSED;
}

int32_t cstring_utils::append_char(char *str, size_t arr_size, char char_to_append)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return CSTRING_UTILS_MEM_ERR;
    }
  
    str[len] = char_to_append;
    str[len + 1] = '\0';
 
    return CSTRING_UTILS_PROCESSED;
}

int32_t cstring_utils::count_trailing_crs_and_lfs(char *str)
{
    if (str == nullptr) {
        return CSTRING_UTILS_MEM_ERR;
    }

    int32_t len = strlen(str);
    if (len == 0) {
        return 0;
    }

    int32_t i = 0;
    while ((str[len - 1] == '\r' || str[len - 1] == '\n') && len > 0) {
        --len;
        ++i;
    }

    return i;
}

int32_t cstring_utils::append_lf_if_no_trailing_crs_nor_lfs(char *str, size_t arr_size)
{
    int32_t count = cstring_utils::count_trailing_crs_and_lfs(str);
    
    if (count < 0) { 
        return CSTRING_UTILS_MEM_ERR;
    } else if (count == 0) {
        return CSTRING_UTILS_NOT_PROCESSED;
    }

    int32_t len = strlen(str);
    if (arr_size - len < 2) {  // One byte for an appended character, another byte for null.
        return CSTRING_UTILS_MEM_ERR;
    }
  
    str[len] = '\n';
    str[len + 1] = '\0';
 
    return CSTRING_UTILS_PROCESSED;
}
