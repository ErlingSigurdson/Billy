/*************** FILE DESCRIPTION ***************/

/**
 * Filename: Drv7Seg4D2x595.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A class for shifting 2-byte data into 2 daisy-chained 74HC595 ICs.
 *           Usually used to drive a multiplexed 4-digit 7-segment display.
 *           Intended for use with the ESP32 or ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    One byte is supposed to have just one bit set and all other bits
 *           cleared. The set bit has to be mapped to a display pin (usually
 *           connected to a respective IC pin via a transistor or a similar
 *           switch) that corresponds to a current active digit.
 *
 *           Another byte is supposed to hold a mapped character, that is,
 *           a combination of set and cleared bits that corresponds to display
 *           segments in a pattern that will provide for an output of a desired
 *           character. This byte may also hold a fully zeroed byte, which
 *           leads to a blank output.
 *
 *           Idiomatic use example:
 *           * * *
 *           // Once (pick just one variant):
 *           driver7seg.init_bb(DRV7SEG4D2X595_DATA_PIN, DRV7SEG4D2X595_LATCH_PIN, DRV7SEG4D2X595_CLOCK_PIN,
 *                              DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY);
 *
 *           driver7seg.init_spi(DRV7SEG4D2X595_MOSI_PIN, DRV7SEG4D2X595_LATCH_PIN, DRV7SEG4D2X595_SCK_PIN,
 *                               DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY);
 *
 *           driver7seg.init_spi(DRV7SEG4D2X595_LATCH_PIN, DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY);
 *
 *
 *           // In a loop:
 *           counter.update();
 *
 *           uint32_t minutes_tens = counter.minutes / 10;
 *           uint32_t minutes_ones = counter.minutes % 10;
 *           uint32_t seconds_tens = counter.seconds / 10;
 *           uint32_t seconds_ones = counter.seconds % 10;
 *
 *           uint8_t digit_1 = mapped_characters[minutes_tens];
 *           uint8_t digit_2 = mapped_characters[minutes_ones];
 *           uint8_t digit_3 = mapped_characters[seconds_tens];
 *           uint8_t digit_4 = mapped_characters[seconds_ones];
 *
 *           if (counter.seconds % 2) {
 *               uint8_t dot_bit_pos_mask = (1 << DRV7SEG4D2X595_DOT_BIT_POS);
 *               digit_2 |= dot_bit_pos_mask;
 *           }
 *
 *           driver7seg.shift_out((1 << DRV7SEG4D2X595_D1), digit_1);
 *           driver7seg.shift_out((1 << DRV7SEG4D2X595_D2), digit_2);
 *           driver7seg.shift_out((1 << DRV7SEG4D2X595_D3), digit_3);
 *           driver7seg.shift_out((1 << DRV7SEG4D2X595_D4), digit_4);
 *           * * *
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
#define DRV7SEG4D2X595_SHIFTING_COMMENCED  0  /* The success of shifting can't be verified programmatically,
                                               * but this return code indicates that the attempt to start
                                               * shifting was begun.
                                               */

#define DRV7SEG4D2X595_BLANK_DIGIT 0b00000000


/****************** DATA TYPES ******************/

class Drv7Seg4D2x595 {
    public:
        /*--- Methods ---*/
        
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
        /*--- Variables ---*/
        
        int32_t  _variant;
        uint32_t _data_pin;
        uint32_t _latch_pin;
        uint32_t _clock_pin;

        /* A duration (in milliseconds) of a tiny pause that prevents the so-called
         * "ghosting" of characters being output to a multiplexed 7-segment display.
         *
         * Usually 2 milliseconds is enough.
         */
        uint32_t _ghosting_prevention_delay;
};


/*************** GLOBAL VARIABLES ***************/

extern Drv7Seg4D2x595 driver7seg;


#endif  // Include guards.
