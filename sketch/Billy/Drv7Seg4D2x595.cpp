/*************** FILE DESCRIPTION ***************/

/**
 * Filename: Drv7Seg4D2x595.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A class for shifting 2-byte data into 2 daisy-chained 74HC595 ICs.
 *           Usually used to drive a multiplexed 4-digit 7-segment display.
 *           Intended for use with the ESP32 or ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "Drv7Seg4D2x595.h"

// Project configs.
#include "config_general.h"

// Arduino core header file.
#include <Arduino.h>

// Additional Arduino libraries.
#include <SPI.h>


/*************** GLOBAL VARIABLES ***************/

Drv7Seg4D2x595 driver7seg;


/******************* FUNCTIONS ******************/

/*--- Constructor ---*/

Drv7Seg4D2x595::Drv7Seg4D2x595()
{
    this->variant = DRV7SEG4D2X595_VARIANT_UNINITIALIZED;
}


/*--- Misc functions ---*/

void Drv7Seg4D2x595::init_bb(uint32_t data_pin, uint32_t latch_pin, uint32_t clock_pin,
                             uint32_t ghosting_prevention_delay)
{
    this->variant = DRV7SEG4D2X595_VARIANT_BIT_BANGING;
    this->data_pin = data_pin;
    this->latch_pin = latch_pin;
    this->clock_pin = clock_pin;
    this->ghosting_prevention_delay = ghosting_prevention_delay;

    pinMode(data_pin,  OUTPUT);
    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
}

void Drv7Seg4D2x595::init_spi(uint32_t latch_pin, uint32_t ghosting_prevention_delay)
{
    this->variant = DRV7SEG4D2X595_VARIANT_SPI;
    this->latch_pin = latch_pin;
    this->ghosting_prevention_delay = ghosting_prevention_delay;

    pinMode(latch_pin, OUTPUT);
    SPI.begin();
}

void Drv7Seg4D2x595::init_spi(uint32_t mosi_pin, uint32_t latch_pin, uint32_t sck_pin,
                              uint32_t ghosting_prevention_delay)
{
    this->variant = DRV7SEG4D2X595_VARIANT_SPI;
    this->latch_pin = latch_pin;
    this->ghosting_prevention_delay = ghosting_prevention_delay;

    pinMode(latch_pin, OUTPUT);
    SPI.begin(sck_pin, -1, mosi_pin, -1);
}

int32_t Drv7Seg4D2x595::shift_out(uint8_t ubyte, uint8_t lbyte)
{
    switch (this->variant) {
        case DRV7SEG4D2X595_VARIANT_UNINITIALIZED:
            return DRV7SEG4D2X595_SHIFTING_PREVENTED;

        case DRV7SEG4D2X595_VARIANT_BIT_BANGING:
            digitalWrite(this->latch_pin, LOW);
            shiftOut(this->data_pin, this->clock_pin, MSBFIRST, ubyte);
            shiftOut(this->data_pin, this->clock_pin, MSBFIRST, lbyte);
            digitalWrite(this->latch_pin, HIGH);

            delay(this->ghosting_prevention_delay);
            digitalWrite(this->latch_pin, LOW);
            // Single byte is enough in this case since it's guaranteed to produce a blank output.
            shiftOut(this->data_pin, this->clock_pin, MSBFIRST, DRV7SEG4D2X595_BLANK_DIGIT);
            digitalWrite(this->latch_pin, HIGH);
            break;

        case DRV7SEG4D2X595_VARIANT_SPI:
            digitalWrite(this->latch_pin, LOW);
            SPI.transfer(ubyte);
            SPI.transfer(lbyte);
            digitalWrite(this->latch_pin, HIGH);

            delay(this->ghosting_prevention_delay);
            digitalWrite(this->latch_pin, LOW);
            // Single byte is enough in this case since it's guaranteed to produce a blank output.
            SPI.transfer(DRV7SEG4D2X595_BLANK_DIGIT);
            digitalWrite(this->latch_pin, HIGH);
            break;

        default:
            break;  // Do nothing and hail MISRA.
    }

    return DRV7SEG4D2X595_SHIFTING_COMMENCED;
}
