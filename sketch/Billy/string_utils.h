/*************** FILE DESCRIPTION ***************/

/**
 * Filename: string_utils.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  General-purpose macros and functions for use with
 *           null-terminated (C-style) strings and similar data.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards
#ifndef STRING_UTILS_H
#define STRING_UTILS_H


/*--- Includes ---*/

// Relevant standard libraries
#include <cstdint>


/*--- Misc ---*/

// A pair of macros for turning an argument into a string.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x

// Function return codes.
#define STRING_UTILS_MEM_ERR       -1
#define STRING_UTILS_NOT_TRIGGERED  0
#define STRING_UTILS_TRIGGERED      1


/************** FUNCTION PROTOTYPES *************/

namespace string_utils {
    int32_t to_lowercase_string(char *buf);
    int32_t to_uppercase_string(char *buf);
    int32_t nullify_first_cr_or_lf_in_string(char *buf);
    int32_t nullify_all_trailing_cr_and_lf_in_string(char *buf);
    int32_t append_cr_to_string(char *buf, size_t buf_size);
    int32_t append_lf_to_string(char *buf, size_t buf_size);
    int32_t append_lf_to_string_if_no_trailing(char *buf, size_t buf_size);
    int32_t nullify_all_cr_and_lf_in_char_array(char *buf, size_t buf_size);
    int32_t substitute_all_cr_and_lf_in_char_array(char *buf, size_t buf_size, char character);
}


#endif  // Include guards.
