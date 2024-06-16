/****************** DESCRIPTION *****************/

/**
 * Filename: setup_wireless.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: setting up wireless connectivity.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/
 
// Include guards.
#ifndef SETUP_WIRELESS_H
#define SETUP_WIRELESS_H


/*--- Includes ---*/

// Local modules.
#include "stored_configs.h"
 
 
/************** FUNCTION PROTOTYPES *************/

void setup_WiFi(stored_configs_t *stored_configs);
void setup_BTClassic(stored_configs_t *stored_configs);


#endif SETUP_WIRELESS_H