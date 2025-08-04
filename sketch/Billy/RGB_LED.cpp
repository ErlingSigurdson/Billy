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

RGBLED *p_Billy_RGB;


/******************* FUNCTIONS ******************/

void RGB_LED_init(uint32_t red_pin, uint32_t green_pin, uint32_t blue_pin, bool is_common_anode)
{
    static RGBLED Billy_RGB(red_pin, green_pin, blue_pin, is_common_anode);
    p_Billy_RGB = &Billy_RGB;
}

void RGB_LED_color_output(const char *cmd_buf)
{
    char *cmd_val = strstr(cmd_buf, "=") + 1;

    if (!strcmp(cmd_val, "RED")) {
        p_Billy_RGB->setColor(RGB::Color::Red);
        return;
    }

    if (!strcmp(cmd_val, "GREEN")) {
        p_Billy_RGB->setColor(RGB::Color::Green);
        return;
    }

    if (!strcmp(cmd_val, "BLUE")) {
        p_Billy_RGB->setColor(RGB::Color::Blue);
        return;
    }

    if (!strcmp(cmd_val, "WHITE")) {
        p_Billy_RGB->setColor(RGB::Color::White);
        return;
    }

    if (!strcmp(cmd_val, "YELLOW")) {
        p_Billy_RGB->setColor(RGB::Color::Yellow);
        return;
    }

    if (!strcmp(cmd_val, "ORANGE")) {
        p_Billy_RGB->setColor(RGB::Color::Orange);
        return;
    }

    if (!strcmp(cmd_val, "PINK")) {
        p_Billy_RGB->setColor(RGB::Color::Pink);
        return;
    }

    if (!strcmp(cmd_val, "MAGENTA")) {
        p_Billy_RGB->setColor(RGB::Color::Magenta);
        return;
    }

    if (!strcmp(cmd_val, "LIME")) {
        p_Billy_RGB->setColor(RGB::Color::Lime);
        return;
    }

    if (!strcmp(cmd_val, "CYAN")) {
        p_Billy_RGB->setColor(RGB::Color::Cyan);
        return;
    }
}
