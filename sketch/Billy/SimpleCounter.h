/****************** DESCRIPTION *****************/

/**
 * Filename: SimpleCounter.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A simple counter. Counts seconds, minutes and hours.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    Just call the update() method in every cycle of your loop() function.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef SIMPLECOUNTER_H
#define SIMPLECOUNTER_H


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>


/****************** DATA TYPES ******************/

class SimpleCounter_t {
    public:
        SimpleCounter_t();
        void update();

        bool launch_flag = 0;
        uint64_t previous_millis = 0;
        uint32_t seconds = 0;
        uint32_t minutes = 0;
        uint32_t hours = 0;
};


/*************** GLOBAL VARIABLES ***************/

extern SimpleCounter_t SimpleCounter;


#endif  // Include guards.
