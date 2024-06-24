/****************** DESCRIPTION *****************/

/**
 * Filename: utilities.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  some general purpose macros and functions.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef UTILITIES_H
#define UTILITIES_H


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>


/*--- Misc ---*/

// A pair of macros for turning an argument into a string.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x


/************** FUNCTION PROTOTYPES *************/

/*--- String operations ---*/

bool utilities_nullify_first_CR_or_LF_in_string(char *buf);
bool utilities_nullify_all_CR_and_LF_in_char_array(char *buf, size_t buf_size);
bool utilities_nullify_all_trailing_CR_and_LF_in_string(char *buf);
bool utilities_substitute_all_CR_and_LF_in_char_array(char *buf, size_t buf_size, char character);
bool utilities_append_LF_to_string(char *buf, size_t buf_size);
bool utilities_append_LF_if_absent_to_string(char *buf, size_t buf_size);
bool utilities_append_CR_to_string(char *buf, size_t buf_size);
void utilities_to_lowercase_string(char *buf);


#endif  // Include guards.
