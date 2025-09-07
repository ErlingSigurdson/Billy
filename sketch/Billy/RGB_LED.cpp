/*************** FILE DESCRIPTION ***************/

/**
 * Filename: RGB_LED.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  RGB LED control.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "RGB_LED.h"

// Arduino core header file.
#include <Arduino.h>  /* I guess I could do with <cstdint> and <cstring>, but I decided to keep it
                       * in case I need to use millis() or something.
                       */

// Additional libraries for Arduino IDE.
#include <RGBLED.h>  // https://github.com/GyverLibs/RGBLED

// Local modules.
#include "cstring_utils.h"


/*************** GLOBAL VARIABLES ***************/

RGBLED *rgb_led_ptr;


/******************* FUNCTIONS ******************/

bool RGB_LED_init(uint32_t red_pin, uint32_t green_pin, uint32_t blue_pin, bool is_common_anode)
{
    static RGBLED _rgb_led(red_pin, green_pin, blue_pin, is_common_anode);
    rgb_led_ptr = &_rgb_led;

    if (rgb_led_ptr) {
        return RGB_LED_SUCCESS;
    } else {
        return RGB_LED_FAILURE;
    }
}

uint32_t RGB_LED_output_color(char *cmd_val)
{
    if (rgb_led_ptr == nullptr) {
        return RGB_LED_FAILURE;
    }

    if (strlen(cmd_val) < 3) {  /* There's no color name shorter than 3 characters,
                                 * so there's a chance for an early return.
                                 */
        return RGB_LED_FAILURE;
    }

    cstring_utils::to_uppercase(cmd_val);

    if (cstring_utils::are_equal(cmd_val, "RED")) {
        rgb_led_ptr->setColor(RGB::Color::Red);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "GREEN")) {
        rgb_led_ptr->setColor(RGB::Color::Green);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "BLUE")) {
        rgb_led_ptr->setColor(RGB::Color::Blue);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "WHITE")) {
        rgb_led_ptr->setColor(RGB::Color::White);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "LIGHTGRAY")) {
        rgb_led_ptr->setColor(RGB::Color::LightGray);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "DARKGRAY")) {
        rgb_led_ptr->setColor(RGB::Color::DarkGray);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "YELLOW")) {
        rgb_led_ptr->setColor(RGB::Color::Yellow);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "GOLD")) {
        rgb_led_ptr->setColor(RGB::Color::Gold);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "ORANGE")) {
        rgb_led_ptr->setColor(RGB::Color::Orange);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "MAROON")) {
        rgb_led_ptr->setColor(RGB::Color::Maroon);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "PINK")) {
        rgb_led_ptr->setColor(RGB::Color::Pink);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "CORAL")) {
        rgb_led_ptr->setColor(RGB::Color::Coral);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "MAGENTA")) {
        rgb_led_ptr->setColor(RGB::Color::Magenta);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "PURPLE")) {
        rgb_led_ptr->setColor(RGB::Color::Purple);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "VIOLET")) {
        rgb_led_ptr->setColor(RGB::Color::Violet);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "LIME")) {
        rgb_led_ptr->setColor(RGB::Color::Lime);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "SKYBLUE")) {
        rgb_led_ptr->setColor(RGB::Color::SkyBlue);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "STEELBLUE")) {
        rgb_led_ptr->setColor(RGB::Color::SteelBlue);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "CYAN")) {
        rgb_led_ptr->setColor(RGB::Color::Cyan);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "TEAL")) {
        rgb_led_ptr->setColor(RGB::Color::Teal);
        return RGB_LED_SUCCESS;
    }

    if (cstring_utils::are_equal(cmd_val, "NAVY")) {
        rgb_led_ptr->setColor(RGB::Color::Navy);
        return RGB_LED_SUCCESS;
    }

    return RGB_LED_FAILURE;
}

void RGB_LED_output_on()
{
    rgb_led_ptr->enable();
}

void RGB_LED_output_off()
{
    rgb_led_ptr->disable();
}
