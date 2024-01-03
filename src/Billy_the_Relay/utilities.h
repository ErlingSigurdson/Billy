/****************** DESCRIPTION *****************/

/**
 * Filename: utilities.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: некоторые вспомогательные макросы и функции.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef UTILITIES_H
#define UTILITIES_H

// Пара макросов для превращения аргумента в строку.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

bool utilities_remove_CR_and_LF(char *buf);
bool utilities_append_CR_and_LF(char *buf, size_t buf_size);
bool utilities_append_LF(char *buf, size_t buf_size);
bool utilities_append_LF_if_absent(char *buf, size_t buf_size);
bool utilities_force_2xLF(char *buf, size_t buf_size);


#endif  // Завершающая директива защиты от повторного включения заголовочного файла.
