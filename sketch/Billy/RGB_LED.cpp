/****************** DESCRIPTION *****************/

/**
 * Filename: RGB_LED.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  RGB LED control.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes: I actually doubted whether I want a separate pair of files for this functionality, but eventually I decided
 *        to have them in case I change the library in use or decide to create some complex wrappers.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>  /* I guess I could do with <cstdint> and <cstring>, but I decided to keep it
                       * in case I need to use millis() or something.
                       */

// Additional libraries for Arduino IDE.
#include <RGBLED.h>  // https://github.com/GyverLibs/RGBLED

// Local modules.
#include "RGB_LED.h"


/*************** GLOBAL VARIABLES ***************/

RGBLED *p_RGB_LED;


/******************* FUNCTIONS ******************/

void RGB_LED_init(uint32_t red_pin, uint32_t green_pin, uint32_t blue_pin, bool is_common_anode)
{
    static RGBLED RGB_LED(red_pin, green_pin, blue_pin, is_common_anode);
    p_RGB_LED = &RGB_LED;
}

uint32_t RGB_LED_output_color(const char *cmd_val)
{
    if (!strcmp(cmd_val, "RED")) {
        p_RGB_LED->setColor(RGB::Color::Red);
        return 0;
    }

    if (!strcmp(cmd_val, "GREEN")) {
        p_RGB_LED->setColor(RGB::Color::Green);
        return 0;
    }

    if (!strcmp(cmd_val, "BLUE")) {
        p_RGB_LED->setColor(RGB::Color::Blue);
        return 0;
    }

    if (!strcmp(cmd_val, "WHITE")) {
        p_RGB_LED->setColor(RGB::Color::White);
        return 0;
    }

    if (!strcmp(cmd_val, "YELLOW")) {
        p_RGB_LED->setColor(RGB::Color::Yellow);
        return 0;
    }

    if (!strcmp(cmd_val, "ORANGE")) {
        p_RGB_LED->setColor(RGB::Color::Orange);
        return 0;
    }

    if (!strcmp(cmd_val, "PINK")) {
        p_RGB_LED->setColor(RGB::Color::Pink);
        return 0;
    }

    if (!strcmp(cmd_val, "MAGENTA")) {
        p_RGB_LED->setColor(RGB::Color::Magenta);
        return 0;
    }

    if (!strcmp(cmd_val, "LIME")) {
        p_RGB_LED->setColor(RGB::Color::Lime);
        return 0;
    }

    if (!strcmp(cmd_val, "CYAN")) {
        p_RGB_LED->setColor(RGB::Color::Cyan);
        return 0;
    }

    return 1;
}
