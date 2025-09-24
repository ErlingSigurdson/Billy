/*************** FILE DESCRIPTION ***************/

/**
 * Filename: Drv7Seg4D2x595.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: A class for shifting 2-byte data into 2 daisy-chained 74HC595 ICs.
 *          Intended for use with the ESP32 or ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef DRV7SEG4D2X595_H
#define DRV7SEG4D2X595_H


/*--- Includes ---*/

// Relevant standard libraries.
#include <cstdint>


/*--- Misc ---*/

// Initialization variant codes.
#define DRV7SEG4D2X595_VARIANT_UNINITIALIZED -1
#define DRV7SEG4D2X595_VARIANT_BIT_BANGING    0
#define DRV7SEG4D2X595_VARIANT_SPI            1

// Function return codes.
#define DRV7SEG4D2X595_SHIFTING_PREVENTED -1
#define DRV7SEG4D2X595_SHIFTING_COMMENCED  0


/****************** DATA TYPES ******************/

class Drv7Seg4D2x595 {
    public:
        Drv7Seg4D2x595();

        // Initialize the driver to use bit-banging with assigned pins.
        void init_bb(uint32_t data_pin, uint32_t latch_pin, uint32_t clock_pin,
                     uint32_t ghosting_prevention_delay = 2);

        // Initialize the driver to use default SPI pins.
        void init_spi(uint32_t latch_pin, uint32_t ghosting_prevention_delay = 2);

        // Initialize the driver to use custom assigned SPI pins.
        void init_spi(uint32_t mosi_pin, uint32_t latch_pin, uint32_t sck_pin,
                      uint32_t ghosting_prevention_delay = 2);

        /* Shift 2-byte data into 2 daisy-chained 74HC595 ICs.
         * Returns: negative integer if the driver is uninitialized, zero otherwise.
         */
        int32_t shift_out(uint8_t ubyte, uint8_t lbyte);

    private:
        int32_t variant;
        uint32_t data_pin;
        uint32_t latch_pin;
        uint32_t clock_pin;

        /* A duration (in milliseconds) of a tiny pause that prevents the so-called
         * "ghosting" of symbols being output to a multiplexed 7-segment display.
         *
         * Usually 2 milliseconds is enough.
         */
        uint32_t ghosting_prevention_delay;
};


/*************** GLOBAL VARIABLES ***************/

extern Drv7Seg4D2x595 driver7seg;


#endif  // Include guards.
