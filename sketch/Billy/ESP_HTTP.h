/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_HTTP.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A simple HTTP server for ESP32/ESP8266.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP_HTTP_H
#define ESP_HTTP_H


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>


/*--- Misc ---*/

// CSS styles.
#define CSS_STYLE_DIV \
"div { \
    width: 500px, \
    height: 250px; \
    margin: 0 auto; \
    margin-top: 80px; \
    text-align: center; \
}"

#define CSS_STYLE_FORM \
"form { \
    margin-bottom: 60px; \
}"

#define CSS_STYLE_LABEL \
"label { \
    font-size: 50px; \
}"

#define CSS_STYLE_P \
"p { \
    margin: 0px; \
    margin-top: 10px; \
    font-size: 30px; \
}"

#define CSS_STYLE_SELECT \
"select { \
    font-size: inherit; \
}"

#define CSS_STYLE_INPUT \
"input { \
    width: 80px; \
    height: 35px; \
    font-size: inherit; \
    text-align: inherit; \
}"

#define CSS_STYLE_BUTTON \
"button { \
    margin-left: 10px; \
    font-size: inherit; \
}"

#define CSS_STYLE_PREV_CMD \
"#prev_cmd { \
    text-align: left; \
    font-size: 30; \
    color: red; \
}"

#define CSS_STYLE_OUTPUT_DISABLED \
"#output_disabled { \
    font-size: 50px; \
    margin-bottom: 60px; \
}"

/************** FUNCTION PROTOTYPES *************/

void ESP_HTTP_server_start();
void ESP_HTTP_set_handlers();
void ESP_HTTP_handle_client_in_loop();
void ESP_HTTP_handle_root();
void ESP_HTTP_handle_not_found();
void ESP_HTTP_handle_ctrl();
String ESP_HTTP_send_HTML(const char *prev_cmd);
void ESP_HTTP_copy_buf(char *buf, uint32_t str_max_len);


#endif  // Include guards.
