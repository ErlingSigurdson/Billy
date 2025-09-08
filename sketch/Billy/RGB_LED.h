/*************** FILE DESCRIPTION ***************/

/**
 * Filename: RGB_LED.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  RGB LED control.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef RGB_LED_H
#define RGB_LED_H


/*--- Includes ---*/

// Arduino core header file.
#include <cstdint>


/*--- Misc ---*/

// Function return codes.
#define RGB_LED_SUCCESS  1
#define RGB_LED_FAILURE  0


/************** FUNCTION PROTOTYPES *************/

bool RGB_LED_init(uint32_t red_pin, uint32_t green_pin, uint32_t blue_pin, bool is_common_anode);
uint32_t RGB_LED_output_color(const char *cmd_val);
void RGB_LED_output_on();
void RGB_LED_output_off();


#endif  // Include guards.
