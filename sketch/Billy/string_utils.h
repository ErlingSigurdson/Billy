/*************** FILE DESCRIPTION ***************/

/**
 * Filename: string_utils.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Several general-purpose macros and functions for use with strings.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards
#ifndef STRING_UTILS_H
#define STRING_UTILS_H


/*--- Includes ---*/


/*--- Misc ---*/

// A pair of macros for turning an argument into a string.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x


/************** FUNCTION PROTOTYPES *************/

namespace string_utils {

    /*--- String operations ---*/

    bool nullify_first_cr_or_lf_in_string(char *buf);
    bool nullify_all_cr_and_lf_in_char_array(char *buf, size_t buf_size);
    bool nullify_all_trailing_cr_and_lf_in_string(char *buf);
    bool substitute_all_cr_and_lf_in_char_array(char *buf, size_t buf_size, char character);
    bool append_lf_to_string(char *buf, size_t buf_size);
    bool append_lf_if_absent_to_string(char *buf, size_t buf_size);
    bool append_cr_to_string(char *buf, size_t buf_size);
    void to_lowercase_string(char *buf);
}

#endif  // Include guards.
