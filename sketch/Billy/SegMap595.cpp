/****************** DESCRIPTION *****************/

/**
 * Filename: SegMap595.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A class for mapping a 74HC595 IC's outputs and
 *           a 7-segment display's segments.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    The init method takes a single argument: a string that must reflect the order of the connections made
             between the parallel outputs of the 74HC595 and the segment control pins of the 7-segment display.
             The string must consist of exactly 7 characters: @, A, B, C, D, E, F and G. Every character corresponds
             to a single segment (@ stands for a dot). The character order must correspond to the order
             of the connections, that is, the first (leftmost) character in the string corresponds to the 7th bit
             of the IC's parallel output (Q7 output), the second character corresponds to the 6th bit (Q6 output), etc.

             Uppercase characters may be substituted for their lowercase counterparts. Any other characters are invalid.
             Duplicating characters in the string is illegal.

             If the string is valid, mapped character bytes (bytes which correspond to the intelligible symbols to be
             output on the 7-segment display) will be placed to the member array named mapped_characters
             in the ascending order (from 0 to Z). Dot bit will be cleared in all mapped character bytes, therefore you
             will have to set this bit in your implementation if necessary (get_dot_bit_pos method can be helpful).
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

int32_t SegMap595_t::init(const char *map_str)
{
    this->status = check_map_str(map_str);

    if (this->status) {
        return this->status;
    }

    this->status = read_map_str();

    if (this->status) {
        return this->status;
    }

    map_characters();

    return this->status;
}

uint32_t SegMap595_t::check_map_str(const char *map_str)
{
    if (strlen(map_str) != SEGMAP595_SEG_NUM) {
        return this->status = SEGMAP595_STATUS_ERR_MAP_STR_LEN;
    }

    strncpy(this->map_str, map_str, SEGMAP595_SEG_NUM);

    for (uint32_t i = 0; i < SEGMAP595_SEG_NUM; ++i) {
        if (this->map_str[i] >= 'a' && this->map_str[i] <= 'g') {
            this->map_str[i] -= SEGMAP595_UPPERCASE_TO_LOWERCASE_ACII_CODE_MARGIN;
        }
    }

    for (uint32_t i = 0; i < SEGMAP595_SEG_NUM; ++i) {
        if (this->map_str[i] < '@' || this->map_str[i] > 'G') {
            return this->status = SEGMAP595_STATUS_ERR_MAP_STR_CHAR;
        }
    }

    char current_char = '@';
    for (uint32_t i = 0; i < SEGMAP595_SEG_NUM; ++i, ++current_char) {
        for (uint32_t j = i + 1; j < SEGMAP595_SEG_NUM; ++j) {
            if (this->map_str[i] == this->map_str[j]) {
                return this->status = SEGMAP595_STATUS_ERR_MAP_STR_DUPLICATION;
            }
        }
    }

    return this->status = SEGMAP595_STATUS_OK;
}

uint32_t SegMap595_t::read_map_str()
{
    uint32_t bit_pos_set = 0;

    char current_char = '@';
    for (uint32_t i = 0; i < SEGMAP595_SEG_NUM; ++i, ++current_char) {
        for (uint32_t j = 0; j < SEGMAP595_SEG_NUM; ++j) {
            if (this->map_str[j] == current_char) {
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
