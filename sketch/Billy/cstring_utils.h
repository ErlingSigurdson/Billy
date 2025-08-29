/*************** FILE DESCRIPTION ***************/

/**
 * Filename: cstring_utils.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A handful of macros and functions for use with
 *           C-style (null-terminated) strings.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CSTRING_UTILS_H
#define CSTRING_UTILS_H


/*--- Includes ---*/

// Relevant standard libraries.
#include <cstdint>
#include <cstring>  // for size_t.


/*--- Misc ---*/

// A pair of macros for turning an argument into a string.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x

// Function return codes.
#define CSTRING_UTILS_MEM_ERR       -1
#define CSTRING_UTILS_NOT_PROCESSED  0
#define CSTRING_UTILS_PROCESSED      1


/************** FUNCTION PROTOTYPES *************/

namespace cstring_utils {
    int32_t to_lowercase(char *str);
    int32_t to_uppercase(char *str);
    int32_t nullify_first_cr_or_lf(char *str);
    int32_t nullify_trailing_crs_and_lfs(char *str);
    int32_t nullify_all_crs_and_lfs(char *str);
    int32_t append_cr(char *str, size_t arr_size);
    int32_t append_lf(char *str, size_t arr_size);
    int32_t append_char(char *str, size_t arr_size, char char_to_append);
    int32_t count_trailing_crs_and_lfs(char *str);
    int32_t append_lf_if_no_trailing_crs_nor_lfs(char *str, size_t arr_size);
}


#endif  // Include guards.
