/****************** DESCRIPTION *****************/

/**
 * Filename: SegMap595.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A class for mapping a 74HC595 IC's outputs and
 *           a 7-segment display's segments.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Relevant standard libraries.
#include <cstring>

// Local modules.
#include "SegMap595.h"


/*************** GLOBAL VARIABLES ***************/

SegMap595_t SegMap595 = SegMap595_t();


/******************* FUNCTIONS ******************/

/*--- Constructor ---*/

SegMap595_t::SegMap595_t()
{
}


/*--- Misc functions ---*/

int32_t SegMap595_t::init(const char* map_str)
{
    this->status = check_map_str(map_str);

    if (this->status) {
        return this->status;
    }

    this->status = read_map_str(map_str);

    if (this->status) {
        return this->status;
    }

    map_characters();

    return this->status;
}

uint32_t SegMap595_t::check_map_str(const char* map_str)
{
    if (strlen(map_str) != SEGMAP595_SEG_NUM) {
        return this->status = SEGMAP595_STATUS_ERR_MAP_STR_LEN;
    }

    for (uint32_t i = 0; i < SEGMAP595_SEG_NUM; ++i) {
        if (map_str[i] < '@' || map_str[i] > 'G') {
            return this->status = SEGMAP595_STATUS_ERR_MAP_STR_CHAR;
        }
    }

    char current_char = '@';
    for (uint32_t i = 0; i < SEGMAP595_SEG_NUM; ++i, ++current_char) {
        for (uint32_t j = i + 1; j < SEGMAP595_SEG_NUM; ++j) {
            if (map_str[i] == map_str[j]) {
                return this->status = SEGMAP595_STATUS_ERR_MAP_STR_DUPLICATION;
            }
        }
    }

    return this->status = SEGMAP595_STATUS_OK;
}

uint32_t SegMap595_t::read_map_str(const char* map_str)
{
    uint32_t bit_pos_set = 0;

    char current_char = '@';
    for (uint32_t i = 0; i < SEGMAP595_SEG_NUM; ++i, ++current_char) {
        for (uint32_t j = 0; j < SEGMAP595_SEG_NUM; ++j) {
            if (map_str[j] == current_char) {
                this->bit_pos[i] = SEGMAP595_MSB - j;
                ++bit_pos_set;
            }
        }
    }

    if (bit_pos_set == SEGMAP595_SEG_NUM) {
        return SEGMAP595_STATUS_OK;
    } else {
        return SEGMAP595_STATUS_ERR_BIT_POS_SET;
    }
}

void SegMap595_t::map_characters()
{
    for (uint32_t i = 0; i < SEGMAP595_CHAR_NUM; ++i) {
        for (uint32_t j = 0; j < SEGMAP595_SEG_NUM; ++j) {
            if (this->mapped_alphabetical[i] << j & SEGMAP595_ONLY_MSB_SET) {
                this->mapped_characters[i] |= 1 << this->bit_pos[j];
            } else {
                this->mapped_characters[i] &= ~(1 << this->bit_pos[j]);
            }
        }
    }
}

uint32_t SegMap595_t::get_dot_bit_pos()
{
    return this->bit_pos[0];  /* Dot (represented by the @ sign) is the first character whose position
                               * is determined when a segment string gets analyzed.
                               */
}
