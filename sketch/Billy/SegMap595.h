/****************** DESCRIPTION *****************/

/**
 * Filename: SegMap595.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A class for mapping a 74HC595 IC's outputs and
 *           a 7-segment display's segments.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef SEGMAP595_H
#define SEGMAP595_H


/*--- Includes ---*/

// Relevant standard libraries.
#include <cstdint>


/*--- Misc ---*/

#define SEGMAP595_SEG_NUM  8
#define SEGMAP595_CHAR_NUM 31

#define SEGMAP595_MSB          7
#define SEGMAP595_ONLY_MSB_SET (1 << SEGMAP595_MSB)

#define SEGMAP595_STATUS_OK                      0
#define SEGMAP595_STATUS_INIT                    -1
#define SEGMAP595_STATUS_ERR_MAP_STR_LEN         1
#define SEGMAP595_STATUS_ERR_MAP_STR_CHAR        2
#define SEGMAP595_STATUS_ERR_MAP_STR_DUPLICATION 3
#define SEGMAP595_STATUS_ERR_BIT_POS_SET         4

// @ABCDEFG, @ is for dot.
#define SEGMAP595_MAP_ALPHABETICAL_0 0b01111110
#define SEGMAP595_MAP_ALPHABETICAL_1 0b00110000
#define SEGMAP595_MAP_ALPHABETICAL_2 0b01101101
#define SEGMAP595_MAP_ALPHABETICAL_3 0b01111001
#define SEGMAP595_MAP_ALPHABETICAL_4 0b00110011
#define SEGMAP595_MAP_ALPHABETICAL_5 0b01011011
#define SEGMAP595_MAP_ALPHABETICAL_6 0b01011111
#define SEGMAP595_MAP_ALPHABETICAL_7 0b01110000
#define SEGMAP595_MAP_ALPHABETICAL_8 0b01111111
#define SEGMAP595_MAP_ALPHABETICAL_9 0b01111011
#define SEGMAP595_MAP_ALPHABETICAL_A 0b01110111
#define SEGMAP595_MAP_ALPHABETICAL_B 0b00011111
#define SEGMAP595_MAP_ALPHABETICAL_C 0b01001110
#define SEGMAP595_MAP_ALPHABETICAL_D 0b00111101
#define SEGMAP595_MAP_ALPHABETICAL_E 0b01001111
#define SEGMAP595_MAP_ALPHABETICAL_F 0b01000111
#define SEGMAP595_MAP_ALPHABETICAL_G 0b01011110
#define SEGMAP595_MAP_ALPHABETICAL_H 0b00110111
#define SEGMAP595_MAP_ALPHABETICAL_I 0b00110000
#define SEGMAP595_MAP_ALPHABETICAL_J 0b00111100
#define SEGMAP595_MAP_ALPHABETICAL_L 0b00001110
#define SEGMAP595_MAP_ALPHABETICAL_N 0b00010101
#define SEGMAP595_MAP_ALPHABETICAL_O 0b00011101
#define SEGMAP595_MAP_ALPHABETICAL_P 0b01100111
#define SEGMAP595_MAP_ALPHABETICAL_Q 0b01110011
#define SEGMAP595_MAP_ALPHABETICAL_R 0b00000101
#define SEGMAP595_MAP_ALPHABETICAL_S 0b01011011
#define SEGMAP595_MAP_ALPHABETICAL_T 0b00001111
#define SEGMAP595_MAP_ALPHABETICAL_U 0b00111110
#define SEGMAP595_MAP_ALPHABETICAL_Y 0b00111011
#define SEGMAP595_MAP_ALPHABETICAL_Z 0b01101101
#define SEGMAP595_MAP_ALPHABETICAL_ALL_CHARS SEGMAP595_MAP_ALPHABETICAL_0, \
                                             SEGMAP595_MAP_ALPHABETICAL_1, \
                                             SEGMAP595_MAP_ALPHABETICAL_2, \
                                             SEGMAP595_MAP_ALPHABETICAL_3, \
                                             SEGMAP595_MAP_ALPHABETICAL_4, \
                                             SEGMAP595_MAP_ALPHABETICAL_5, \
                                             SEGMAP595_MAP_ALPHABETICAL_6, \
                                             SEGMAP595_MAP_ALPHABETICAL_7, \
                                             SEGMAP595_MAP_ALPHABETICAL_8, \
                                             SEGMAP595_MAP_ALPHABETICAL_9, \
                                             SEGMAP595_MAP_ALPHABETICAL_A, \
                                             SEGMAP595_MAP_ALPHABETICAL_B, \
                                             SEGMAP595_MAP_ALPHABETICAL_C, \
                                             SEGMAP595_MAP_ALPHABETICAL_D, \
                                             SEGMAP595_MAP_ALPHABETICAL_E, \
                                             SEGMAP595_MAP_ALPHABETICAL_F, \
                                             SEGMAP595_MAP_ALPHABETICAL_G, \
                                             SEGMAP595_MAP_ALPHABETICAL_H, \
                                             SEGMAP595_MAP_ALPHABETICAL_I, \
                                             SEGMAP595_MAP_ALPHABETICAL_J, \
                                             SEGMAP595_MAP_ALPHABETICAL_L, \
                                             SEGMAP595_MAP_ALPHABETICAL_N, \
                                             SEGMAP595_MAP_ALPHABETICAL_O, \
                                             SEGMAP595_MAP_ALPHABETICAL_P, \
                                             SEGMAP595_MAP_ALPHABETICAL_Q, \
                                             SEGMAP595_MAP_ALPHABETICAL_R, \
                                             SEGMAP595_MAP_ALPHABETICAL_S, \
                                             SEGMAP595_MAP_ALPHABETICAL_T, \
                                             SEGMAP595_MAP_ALPHABETICAL_U, \
                                             SEGMAP595_MAP_ALPHABETICAL_Y, \
                                             SEGMAP595_MAP_ALPHABETICAL_Z

#define SEGMAP595_CHAR_0 0
#define SEGMAP595_CHAR_1 1
#define SEGMAP595_CHAR_2 2
#define SEGMAP595_CHAR_3 3
#define SEGMAP595_CHAR_4 4
#define SEGMAP595_CHAR_5 5
#define SEGMAP595_CHAR_6 6
#define SEGMAP595_CHAR_7 7
#define SEGMAP595_CHAR_8 8
#define SEGMAP595_CHAR_9 9
#define SEGMAP595_CHAR_A 10
#define SEGMAP595_CHAR_B 11
#define SEGMAP595_CHAR_C 12
#define SEGMAP595_CHAR_D 13
#define SEGMAP595_CHAR_E 14
#define SEGMAP595_CHAR_F 15
#define SEGMAP595_CHAR_G 16
#define SEGMAP595_CHAR_H 17
#define SEGMAP595_CHAR_I 18
#define SEGMAP595_CHAR_J 19
#define SEGMAP595_CHAR_L 20
#define SEGMAP595_CHAR_N 21
#define SEGMAP595_CHAR_O 22
#define SEGMAP595_CHAR_P 23
#define SEGMAP595_CHAR_Q 24
#define SEGMAP595_CHAR_R 25
#define SEGMAP595_CHAR_S 26
#define SEGMAP595_CHAR_T 27
#define SEGMAP595_CHAR_U 28
#define SEGMAP595_CHAR_Y 29
#define SEGMAP595_CHAR_Z 30


/****************** DATA TYPES ******************/

class SegMap595_t {
    public:
        int32_t status = SEGMAP595_STATUS_INIT;
        uint8_t mapped_characters[SEGMAP595_CHAR_NUM] = {0};

        SegMap595_t();
        int32_t init(const char* map_str);
        uint32_t get_dot_bit_pos();

    private:
        uint8_t mapped_alphabetical[SEGMAP595_CHAR_NUM] = {SEGMAP595_MAP_ALPHABETICAL_ALL_CHARS};
        uint32_t bit_pos[SEGMAP595_SEG_NUM] = {0};

        uint32_t check_map_str(const char* map_str);
        uint32_t read_map_str(const char* map_str);
        void map_characters();
};


/*************** GLOBAL VARIABLES ***************/

extern SegMap595_t SegMap595;


#endif  // Include guards.
