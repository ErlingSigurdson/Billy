/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_HTTP.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: a simplistic HTTP-server for ESP32/ESP8266.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP_HTTP_H
#define ESP_HTTP_H


/*--- Includes ---*/

// Project configs.
#include "config_general.h"
#include "config_ASCII_cmd_check.h"


/************** FUNCTION PROTOTYPES *************/

void ESP_HTTP_server_start();
void ESP_HTTP_handle_client_in_loop();
void ESP_HTTP_handle_root();
void ESP_HTTP_handle_not_found();
void ESP_HTTP_handle_ctrl();
void ESP_HTTP_copy_value(char *buf, uint32_t str_max_len);
String ESP_HTTP_send_HTML(uint32_t previous_cmd);
void ESP_HTTP_set_handlers();


#endif  // Include guards.
