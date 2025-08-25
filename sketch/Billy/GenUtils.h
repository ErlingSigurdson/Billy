/*************** FILE DESCRIPTION ***************/

/**
 * Filename: GenUtils.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Some general purpose macros and functions.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef GENUTILS_H
#define GENUTILS_H


/*--- Includes ---*/

// Relevant standard libraries.
#include <cstdint>
#include <cstring>


/*--- Misc ---*/

// A pair of macros for turning an argument into a string.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x


/************** FUNCTION PROTOTYPES *************/

namespace GenUtils {

    /*--- String operations ---*/

    bool nullify_first_CR_or_LF_in_string(char *buf);
    bool nullify_all_CR_and_LF_in_char_array(char *buf, size_t buf_size);
    bool nullify_all_trailing_CR_and_LF_in_string(char *buf);
    bool substitute_all_CR_and_LF_in_char_array(char *buf, size_t buf_size, char character);
    bool append_LF_to_string(char *buf, size_t buf_size);
    bool append_LF_if_absent_to_string(char *buf, size_t buf_size);
    bool append_CR_to_string(char *buf, size_t buf_size);
    void to_lowercase_string(char *buf);
}

#endif  // Include guards.
