/****************** DESCRIPTION *****************/

/**
 * Filename: utilities.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: некоторые вспомогательные макросы и функции.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef UTILITIES_H
#define UTILITIES_H

// Пара макросов для превращения аргумента в строку.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x


/************** FUNCTION PROTOTYPES *************/

bool utilities_remove_CR_and_LF(char *buf);
bool utilities_append_CR_and_LF(char *buf, size_t buf_size);
bool utilities_append_LF(char *buf, size_t buf_size);
bool utilities_append_LF_if_absent(char *buf, size_t buf_size);
bool utilities_force_2xLF(char *buf, size_t buf_size);


#endif  // Include guards.
