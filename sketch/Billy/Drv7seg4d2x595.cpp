/****************** DESCRIPTION *****************/

/**
 * Filename: Drv7seg4d2x595.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Additional libraries for Arduino IDE.
#include <SPI.h>

// Local modules.
#include "Drv7seg4d2x595.h"
#include "config_general.h"


/*************** GLOBAL VARIABLES ***************/

Drv7seg4d2x595 Daisy16 = Drv7seg4d2x595();


/******************* FUNCTIONS ******************/

Drv7seg4d2x595::Drv7seg4d2x595()
{
}

void Drv7seg4d2x595::init_bb(uint32_t data_pin, uint32_t latch_pin, uint32_t clock_pin, \
                             uint32_t ghosting_prevention_delay)
{
    this->variant = DRV7SEG4D2X595_VARIANT_BIT_BANGING;
    this->data_pin = data_pin;
    this->latch_pin = latch_pin;
    this->clock_pin = clock_pin;
    this->ghosting_prevention_delay = ghosting_prevention_delay;

    pinMode(data_pin, OUTPUT);
    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
}

void Drv7seg4d2x595::init_spi(uint32_t mosi_pin, uint32_t latch_pin, uint32_t sck_pin, \
                              uint32_t ghosting_prevention_delay)
{
    this->variant = DRV7SEG4D2X595_VARIANT_SPI;
    this->latch_pin = latch_pin;
    this->ghosting_prevention_delay = ghosting_prevention_delay;

    pinMode(latch_pin, OUTPUT);
    SPI.begin(sck_pin, -1, mosi_pin, -1);
}

void Drv7seg4d2x595::init_spi(uint32_t latch_pin, uint32_t ghosting_prevention_delay)
{
    this->variant = DRV7SEG4D2X595_VARIANT_SPI;
    this->latch_pin = latch_pin;
    this->ghosting_prevention_delay = ghosting_prevention_delay;

    pinMode(latch_pin, OUTPUT);
    SPI.begin();
}

void Drv7seg4d2x595::shift_out(uint8_t ubyte, uint8_t lbyte)
{
    switch (this->variant) {
        case DRV7SEG4D2X595_VARIANT_BIT_BANGING:
            digitalWrite(latch_pin, 0);
            shiftOut(data_pin, clock_pin, MSBFIRST, ubyte);
            shiftOut(data_pin, clock_pin, MSBFIRST, lbyte);
            digitalWrite(latch_pin, 1);

            delay(this->ghosting_prevention_delay);
            digitalWrite(latch_pin, 0);
            shiftOut(data_pin, clock_pin, MSBFIRST, 0);
            digitalWrite(latch_pin, 1);
            break;

        case DRV7SEG4D2X595_VARIANT_SPI:
            digitalWrite(latch_pin, 0);
            SPI.transfer(ubyte);
            SPI.transfer(lbyte);
            digitalWrite(latch_pin, 1);

            delay(this->ghosting_prevention_delay);
            digitalWrite(latch_pin, 0);
            SPI.transfer(0);
            digitalWrite(latch_pin, 1);
            break;

        default:
            break;  // Do nothing and hail MISRA.
    }
}
