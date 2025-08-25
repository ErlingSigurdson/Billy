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


/************** FUNCTION PROTOTYPES *************/

namespace string_utils {
    void to_lowercase_string(char *buf);
    void to_uppercase_string(char *buf);
    bool nullify_first_cr_or_lf_in_string(char *buf);
    uint32_t nullify_all_trailing_cr_and_lf_in_string(char *buf);
    bool append_cr_to_string(char *buf, size_t buf_size);
    bool append_lf_to_string(char *buf, size_t buf_size);
    int32_t append_lf_to_string_if_no_trailing(char *buf, size_t buf_size);
    uint32_t nullify_all_cr_and_lf_in_char_array(char *buf, size_t buf_size);
    uint32_t substitute_all_cr_and_lf_in_char_array(char *buf, size_t buf_size, char character);
}


#endif  // Include guards.
