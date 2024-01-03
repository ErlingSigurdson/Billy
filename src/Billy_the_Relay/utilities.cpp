/****************** DESCRIPTION *****************/

/**
 * Имя файла: utilities.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: некоторые вспомогательные макросы и функции.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Основная библиотека Arduino.
#include <Arduino.h>

// Локальные модули.
#include "utilities.h"


/******************** ФУНКЦИИ *******************/

bool utilities_remove_CR_and_LF(char *buf)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';

            return 1;
        }
    }

    return 0;
}

bool utilities_append_CR_and_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 3) {  // Один байт для CR, второй для LF, третий для нуля.
        buf[strlen(buf)] = '\r';
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}

bool utilities_append_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}

bool utilities_append_LF_if_absent(char *buf, size_t buf_size)
{
    if (strrchr(buf, '\n') == &buf[strlen(buf) - 1]) {
        return 0;
    } else if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        utilities_append_LF(buf, buf_size);

        return 1;
    }

    return 0;
}

bool utilities_force_2xLF(char *buf, size_t buf_size)
{
    utilities_remove_CR_and_LF(buf);
    if (buf_size - strlen(buf) >= 3) {  // Два байта для LF, третий для нуля.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}
