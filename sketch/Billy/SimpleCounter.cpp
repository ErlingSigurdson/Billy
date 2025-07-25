/****************** DESCRIPTION *****************/

/**
 * Filename: SimpleCounter.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A simple counter. Counts seconds, minutes and hours.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    Just call the update() method in every cycle of your loop() function.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Local modules.
#include "SimpleCounter.h"


/*************** GLOBAL VARIABLES ***************/

SimpleCounter_t SimpleCounter = SimpleCounter_t();


/******************* FUNCTIONS ******************/

/*--- Constructor ---*/

SimpleCounter_t::SimpleCounter_t()
{
}


/*--- Misc functions ---*/

void SimpleCounter_t::update()
{
    uint64_t current_millis = millis();

    if (!this->launch_flag) {
        this->previous_millis = current_millis;
        this->launch_flag = 1;
    }

    if (current_millis - this->previous_millis >= 1000) {
        this->seconds += (current_millis - this->previous_millis) / 1000;
        this->previous_millis = current_millis;
    }

    if (this->seconds > 59) {
        this->minutes += this->seconds / 60;
        this->seconds = this->seconds % 60;
    }

    if (this->minutes > 59) {
        this->hours += this->minutes / 60;
        this->minutes = this->minutes % 60;
    }

    if (this->hours > 24) {
        this->hours = this->hours % 24;
    }
}
