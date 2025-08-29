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
#define STRING_UTILS_NOT_PROCESSED  0
#define STRING_UTILS_PROCESSED      1


/************** FUNCTION PROTOTYPES *************/

namespace string_utils {
    int32_t to_lowercase(char *str);
    int32_t to_uppercase(char *str);
    int32_t nullify_first_cr_or_lf(char *str);
    int32_t nullify_trailing_crs_and_lfs(char *str);
    int32_t nullify_all_crs_and_lfs(char *str);
    int32_t append_cr(char *str, size_t arr_size);
    int32_t append_lf(char *str, size_t arr_size);
    int32_t append_char(char *str, size_t arr_size, char char_to_append);
    int32_t append_lf_if_no_trailing(char *str, size_t arr_size);

    namespace char_array_utils {
        int32_t nullify_all_cr_and_lf_in_char_array(char *str, size_t arr_size);
        int32_t substitute_all_cr_and_lf_in_char_array(char *str, size_t arr_size, char character);
    }
}


#endif  // Include guards.
