/*************** FILE DESCRIPTION ***************/

/**
 * Filename: ESP_HTTP.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A simple HTTP server for ESP32/ESP8266.
 *           Written for use with the ESP32/ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Source file's own header.
#include "ESP_HTTP.h"

// Essential Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"
#include "config_cmd.h"

// Additional libraries for Arduino IDE.
#ifdef ESP32
    #include <WebServer.h>
#endif
#ifdef ESP8266
    #include <ESP8266WebServer.h>
#endif


/*************** GLOBAL VARIABLES ***************/

#ifdef ESP32
    WebServer HTTP_server(HTTP_PORT);
#endif
#ifdef ESP8266
    ESP8266WebServer HTTP_server(HTTP_PORT);
#endif

char ESP_HTTP_buf[STR_MAX_LEN + 1] = {0};


/******************* FUNCTIONS ******************/

void ESP_HTTP_server_start()
{
    HTTP_server.begin();
}

void ESP_HTTP_set_handlers()
{
    // Assigning HTTP request handler functions to particular URIs.
    HTTP_server.on("/", ESP_HTTP_handle_root);
    HTTP_server.onNotFound(ESP_HTTP_handle_not_found);
    HTTP_server.on("/ctrl", ESP_HTTP_handle_ctrl);
}

void ESP_HTTP_handle_client_in_loop()
{
    // Receive and handle HTTP requests.
    HTTP_server.handleClient();
}

void ESP_HTTP_handle_root()
{
    HTTP_server.send(200, "text/html", ESP_HTTP_send_HTML(""));
}

void ESP_HTTP_handle_not_found()
{
    HTTP_server.send(404, "text/plain", "Not found, chummer. Try another page.");
}

void ESP_HTTP_handle_ctrl()
{
    char cmd_1[STR_MAX_LEN + 1] = CMD_1;  // Digital output command.
    cmd_1[strlen(cmd_1) - 1] = '\0';      // Remove an equality sign.

    char cmd_2[STR_MAX_LEN + 1] = CMD_2;  // PWM output command.
    cmd_2[strlen(cmd_2) - 1] = '\0';      // Remove an equality sign.

    if (HTTP_server.hasArg(cmd_1)) {  // Digital output command was received.
        if (DIGITAL_OUTPUT_PIN <= 0) {
            HTTP_server.send(200, "text/plain", "Digital output disabled.");
            return;
        }

        if (HTTP_server.arg(cmd_1).length() > STR_MAX_LEN) {
            HTTP_server.send(200, "text/plain", "Command buffer overflow.");
            return;
        }

        char val[STR_MAX_LEN + 1] = {0};
        strcpy(val, HTTP_server.arg(cmd_1).c_str());

        bool val_is_loaddigital_on  = (!strcmp(val, "ON"));
        bool val_is_loaddigital_off = (!strcmp(val, "OFF"));

        if (!val_is_loaddigital_on && !val_is_loaddigital_off) {
            HTTP_server.send(200, "text/plain", "No valid value submitted.");
            return;
        }

        // Passing the valid command and its value to the main buffer.
        strcpy(ESP_HTTP_buf, CMD_PREFIX);
        strcat(ESP_HTTP_buf, CMD_1);
        strcat(ESP_HTTP_buf, val);

        HTTP_server.send(200, "text/html", ESP_HTTP_send_HTML(val));

        return;
    } else if (HTTP_server.hasArg(cmd_2)) {  // PWM output command was received.
        if (PWM_OUTPUT_PIN <= 0) {
            HTTP_server.send(200, "text/plain", "PWM output disabled.");
            return;
        }

        uint32_t val_len = HTTP_server.arg(cmd_2).length();

        if (val_len < 1 || val_len > 3) {  // Valid duty cycle values are 0 to 255.
            HTTP_server.send(200, "text/plain", "No valid value submitted.");
            return;
        }

        char val[STR_MAX_LEN + 1] = {0};
        strcpy(val, HTTP_server.arg(cmd_2).c_str());

        for (uint32_t i = 0; i < (uint32_t)strlen(val); ++i) {
            if (val[i] < '0' || val[i] > '9') {
                HTTP_server.send(200, "text/plain", "No valid value submitted.");
                return;
            }
        }

        uint32_t duty_cycle = strtol(val, 0, 10);     // Convert to decimal.
        if (duty_cycle > PWM_DUTY_CYCLE_MAX_VALUE) {  // Valid duty cycle values are 0 to 255.
            HTTP_server.send(200, "text/plain", "No valid value submitted.");
            return;
        }

        // Passing the valid command and its value to the main buffer.
        strcpy(ESP_HTTP_buf, CMD_PREFIX);
        strcat(ESP_HTTP_buf, CMD_2);
        strcat(ESP_HTTP_buf, val);

        HTTP_server.send(200, "text/html", ESP_HTTP_send_HTML(val));

        return;
    } else {
        HTTP_server.send(200, "text/plain", "No valid command entered.");
    }
}

String ESP_HTTP_send_HTML(const char *prev_cmd_val)
{
    String _prev_cmd_val = String(prev_cmd_val);

    String site = "";

    site+=HTML_DOCTYPE;
    site+="<html>\n";

        site+="<head>";
            site+=HTML_META;
            site+="<style>";
                site+=CSS_STYLE_BODY;
                site+=CSS_STYLE_DIV;
                site+=CSS_STYLE_FORM;
                site+=CSS_STYLE_LABEL;
                site+=CSS_STYLE_P;
                site+=CSS_STYLE_SELECT;
                site+=CSS_STYLE_INPUT;
                site+=CSS_STYLE_BUTTON;
                site+=CSS_STYLE_PREV_CMD_ON;
                site+=CSS_STYLE_PREV_CMD_OFF;
                site+=CSS_STYLE_PREV_CMD_NEUTRAL;
                site+=CSS_STYLE_OUTPUT_SECTION;
                site+=CSS_STYLE_OUTPUT_DISABLED;
                site+=CSS_STYLE_SQUARE;
                site+=CSS_STYLE_SQUARE_ON;
                site+=CSS_STYLE_SQUARE_OFF;
            site+="</style>";
        site+="</head>";

        site+="<body>";

            if (_prev_cmd_val.length() != 0) {
            // If there was a previous command.
                site+="<p class=";
                    if (_prev_cmd_val == "ON") {
                    // If a two-state load ON value was passed.
                        site+="\"prev_cmd_on\">";
                        site+="Two-state load is ";
                    } else if (_prev_cmd_val == "OFF") {
                    // If a two-state load OFF value was passed.
                        site+="\"prev_cmd_off\">";
                        site+="Two-state load is ";
                    } else {
                    // If a PWM duty cycle value was passed.
                        site+="\"prev_cmd_neutral\">";
                        site+="PWM duty cycle value is set to ";
                    }
                    site+=String(prev_cmd_val);
                site+="</p>";
            } else {
              // If there was no previous command (if the root page is to be served).
                  /* Insert a rather unnecessary paragraph so that the web page contents wouldn't drift
                   * based on the web page variant (depending on whether there was a previous command).
                   */
                  site+="<p class=\"prev_cmd_neutral\">Billy's load control web interface</p>";
            }

            site+="<div>";
                if (DIGITAL_OUTPUT_PIN > 0) {
                    site+="<form action=\"/ctrl\" method=\"POST\" class=\"output_section\">";
                        site+="<button type=\"submit\" name=\"LOADDIGITAL\" value=\"ON\" \
                               class=\"square square_on\">ON</button>";
                        site+="<button type=\"submit\" name=\"LOADDIGITAL\" value=\"OFF\" \
                               class=\"square square_off\">OFF</button>";

                        /* Deprecated variant. GET requests with payload are possible,
                         * but considered to be a bad practice.
                         */
                        //site+="<a href=\"/ctrl?LOADDIGITAL=ON\" class=\"square square_on\">ON</a>";
                        //site+="<a href=\"/ctrl?LOADDIGITAL=OFF\" class=\"square square_off\">OFF</a>";
                    site+="</form>";
                } else {
                    site+="<p class=\"output_disabled\">Digital control disabled</p>";
                }

                site+="<hr>";

                if (PWM_OUTPUT_PIN > 0) {
                    site+="<form action=\"/ctrl\" method=\"POST\" class=\"output_section\">";
                        site+="<label for=\"LOADPWM\">PWM control</label>";

                        // Pure HTML slider was a disappointment.
                        /*
                        site+=" \
                                  <input name=\"LOADPWM\" id=\"LOADPWM\" \
                                  type=\"range\" min=\"0\" \
                                  max=\"" \
                                      STRINGIFY(PWM_DUTY_CYCLE_MAX_VALUE) \
                                  "\"" \
                                  "step=\"1\" value=\"0\" \
                              ";
                        */

                        site+="<p>(insert value from 0 to 255)</p>";
                        site+="<p>";
                            site+="<input name=\"LOADPWM\" id=\"LOADPWM\" type=\"text\">";
                            site+="<button type=\"submit\">Send</button>";
                        site+="</p>";
                    site+="</form>";
                } else {
                    site+="<p class=\"output_disabled\">PWM control disabled</p>";
                }
            site+="</div>";
        site+="</body>";

    site+="</html>";

    return site;
}

void ESP_HTTP_copy_buf(char *buf, uint32_t str_max_len)
{
    if (ESP_HTTP_buf[0] != '\0' && strlen(ESP_HTTP_buf) <= str_max_len) {
        strcpy(buf, ESP_HTTP_buf);
    }

    ESP_HTTP_buf[0] = '\0';
}
