/*************** FILE DESCRIPTION ***************/

/**
 * Filename: minimal_use.ino
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  This is the main file of your sketch, whatever it is and whatever
 *           you will name it. It contains minimal use of Billy's API that
 *           provides interface and command handling.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Billy.
#include "Billy.h"


/******************* FUNCTIONS ******************/

void setup()
{
    billy::main::in_setup();
}

void loop()
{
    billy::main::in_loop();
}
