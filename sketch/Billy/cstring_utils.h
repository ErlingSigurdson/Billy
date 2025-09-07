/*************** FILE DESCRIPTION ***************/

/**
 * Filename: cstring_utils.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A collection of macros and functions for inspecting and processing
 *           C-style (null-terminated) strings.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    All functions with a bool return type will return false
 *           not only if a tested criterion (implied by a function's name)
 *           is not met, but also if a passed pointer to char or const char
 *           is nullptr.
 *
 *           All functions with a signed integer return type will return:
 *           * -1 - if a passed pointer to char or const char equals nullptr
 *                  or another memory-related issue was encountered.
 *           *  0 - if there's nothing to process in a string.
 *           *  A positive integer otherwise (see comments for a particular
 *              function).
 *
 *           Functions whose names imply processing CRs and LFs won't process
 *           strings which consist of CRs and/or LFs only (that counts as
 *           "nothing to process in a string").
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CSTRING_UTILS_H
#define CSTRING_UTILS_H


/*--- Includes ---*/

// Relevant standard libraries.
#include <cstddef>  // For size_t.
#include <cstdint>


/*--- Misc ---*/

// A pair of macros for turning an argument into a string.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x

// Function return codes.
#define CSTRING_UTILS_ERR_MEM       -1
#define CSTRING_UTILS_NOT_PROCESSED  0
#define CSTRING_UTILS_PROCESSED      1


/************** FUNCTION PROTOTYPES *************/

namespace cstring_utils {

    /*--- Inspection functions ---*/

    bool    is_string(const char *arr, size_t arr_size);
    bool    are_equal(const char *str1, const char *str2);

    // Both return a count.
    int32_t count_leading_crs_and_lfs(const char *str);
    int32_t count_trailing_crs_and_lfs(const char *str);

    
    /*--- Processing functions ---*/

                                                                           // If a string is processed, returns:
                                                    
    int32_t append_char(char *str, size_t arr_size, char char_to_append);  // 1
    
    int32_t to_lowercase(char *str);                                       // Number of characters converted.
    
    int32_t to_lowercase_until_char(char *str, char stopper);              // Number of characters converted.
    
    int32_t to_uppercase(char *str);                                       // Number of characters converted.
    
    int32_t to_uppercase_until_char(char *str, char stopper);              // Number of characters converted.
    
    int32_t nullify_first_cr_or_lf(char *str);                             // 1
    
    int32_t trim_leading_crs_and_lfs(char *str);                           // Number of bytes trimmed.
    
    int32_t inner_cr_and_lf_groups_to_single_spaces(char *str);            // Number of groups converted.
    
    int32_t cut_off_trailing_crs_and_lfs(char *str);                       // 1
    
    int32_t to_single_line(char *str);                                     /* Number of processing stages performed
                                                                            * (up to 3).
                                                                            */
}


#endif  // Include guards.
