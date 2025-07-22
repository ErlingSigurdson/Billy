/****************** DESCRIPTION *****************/

/**
 * Filename: Drv7seg4d2x595.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: A class for shifting 2-byte data into 2 daisy-chained 74HC595 ICs.
 *          Written for use with the ESP32/ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef DRV7SEG4D2X595_H
#define DRV7SEG4D2X595_H


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>


/*--- Misc ---*/

#define DRV7SEG4D2X595_VARIANT_BIT_BANGING 0
#define DRV7SEG4D2X595_VARIANT_SPI         1


/****************** DATA TYPES ******************/

class Drv7seg4d2x595_t {
    public:
        Drv7seg4d2x595_t();
        void init_bb(uint32_t data_pin, uint32_t latch_pin, uint32_t clock_pin, \
                     uint32_t ghosting_prevention_delay);
        void init_spi(uint32_t mosi_pin, uint32_t latch_pin, uint32_t sck_pin, \
                      uint32_t ghosting_prevention_delay);
        void init_spi(uint32_t latch_pin, uint32_t ghosting_prevention_delay);
        void shift_out(uint8_t ubyte, uint8_t lbyte);

    private:
        uint32_t variant;
        uint32_t data_pin;
        uint32_t mosi_pin;
        uint32_t latch_pin;
        uint32_t clock_pin;
        uint32_t sck_pin;
        uint32_t ghosting_prevention_delay;
};


/*************** GLOBAL VARIABLES ***************/

extern Drv7seg4d2x595_t Drv7seg4d2x595;


#endif  // Include guards.
