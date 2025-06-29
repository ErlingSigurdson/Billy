/****************** DESCRIPTION *****************/

/**
 * Filename: ESP32_BTClassic.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Bluetooth Classic wrapper functions for ESP32 SoC.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP32_BTCLASSIC_H
#define ESP32_BTCLASSIC_H


/*--- Misc ---*/

#define BTCLASSIC_READ_SLOWDOWN 2


/************** FUNCTION PROTOTYPES *************/

void ESP32_BTClassic_start(const char *dev_name);
bool ESP32_BTClassic_check_connection();
uint32_t ESP32_BTClassic_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
void ESP32_BTClassic_send_msg(const char *msg);
void ESP32_BTClassic_disconnect(uint32_t shutdown_downtime);
void ESP32_BTClassic_stop(uint32_t shutdown_downtime);


#endif  // Include guards.
