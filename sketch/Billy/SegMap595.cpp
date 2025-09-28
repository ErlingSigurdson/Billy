/*************** FILE DESCRIPTION ***************/

/**
 * Filename: SegMap595.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A class for mapping the outputs of a 74HC595 IC
 *           to the segments of a 7-segment display.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "SegMap595.h"

// Relevant standard libraries.
#include <cstring>


/*************** GLOBAL VARIABLES ***************/

SegMap595 segmap595;


/******************* FUNCTIONS ******************/

/*--- Constructor ---*/

SegMap595::SegMap595()
{
}


/*--- Misc functions ---*/

int32_t SegMap595::init(const char *map_str)
{
    _status = check_map_str(map_str);

    if (_status < 0) {
        return _status;
    }

    _status = read_map_str();

    if (_status < 0) {
        return _status;
    }

    map_characters();

    return _status;
}

int32_t SegMap595::check_map_str(const char *map_str)
{
    if (map_str == nullptr) {
        return SEGMAP595_STATUS_ERR_NULLPTR;
    }

    size_t str_len = strlen(map_str);
    if (str_len != SEGMAP595_SEG_NUM) {
        return _status = SEGMAP595_STATUS_ERR_MAP_STR_LEN;
    }

    strncpy(_map_str, map_str, SEGMAP595_SEG_NUM);

    // Convert to uppercase.
    constexpr int32_t ascii_code_diff = 'a' - 'A';
    for (size_t i = 0; i < SEGMAP595_SEG_NUM; ++i) {
        if (_map_str[i] >= 'a' && _map_str[i] <= 'g') {
            _map_str[i] -= ascii_code_diff;
        }
    }

    // Check for invalid characters.
    for (size_t i = 0; i < SEGMAP595_SEG_NUM; ++i) {
        if (_map_str[i] < '@' || _map_str[i] > 'G') {
            return _status = SEGMAP595_STATUS_ERR_MAP_STR_CHAR;
        }
    }

    // Check for character duplication.
    for (size_t i = 0; i < SEGMAP595_SEG_NUM; ++i) {
        for (size_t j = i + 1; j < SEGMAP595_SEG_NUM; ++j) {
            if (_map_str[i] == _map_str[j]) {
                return _status = SEGMAP595_STATUS_ERR_MAP_STR_DUPLICATION;
            }
        }
    }

    return _status = SEGMAP595_STATUS_OK;
}

int32_t SegMap595::read_map_str()
{
    int32_t bit_pos_set = 0;

    char current_char = '@';
    for (size_t i = 0; i < SEGMAP595_SEG_NUM; ++i, ++current_char) {
        for (size_t j = 0; j < SEGMAP595_SEG_NUM; ++j) {
            if (_map_str[j] == current_char) {
                _bit_pos[i] = SEGMAP595_MSB - j;
                ++bit_pos_set;
            }
        }
    }

    // Bit position must be set for all segments. If not, that's an error.
    if (bit_pos_set == SEGMAP595_SEG_NUM) {
        return SEGMAP595_STATUS_OK;
    } else {
        return SEGMAP595_STATUS_ERR_BIT_POS_SET;
    }
}

void SegMap595::map_characters()
{
    for (size_t i = 0; i < SEGMAP595_CHAR_NUM; ++i) {
        for (size_t j = 0; j < SEGMAP595_SEG_NUM; ++j) {
            if ((_mapped_alphabetical[i] << j) & SEGMAP595_ONLY_MSB_SET) {
                mapped_characters[i] |=  (1 << _bit_pos[j]);
            } else {
                mapped_characters[i] &= ~(1 << _bit_pos[j]);
            }
        }
    }
}

uint32_t SegMap595::get_dot_bit_pos()
{
    return _bit_pos[0];  /* Dot (represented by the @ sign) is the first character
                          * whose position is determined when a segment string gets analyzed.
                          */
}

int32_t SegMap595::get_status()
{
    return _status;
}

const char* SegMap595::get_map_str()
{
    if (_status >= 0) {  // If character mapping was successful.
        return _map_str;
    } else {
        return nullptr;
    }
}
