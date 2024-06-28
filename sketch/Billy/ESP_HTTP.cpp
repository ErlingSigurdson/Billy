/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_HTTP.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  A simple HTTP server for ESP32/ESP8266.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

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

// Local modules.
#include "ESP_HTTP.h"


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
    HTTP_server.send(200, "text/html", ESP_HTTP_send_HTML(NO_PREVIOUS_CMD));
}

void ESP_HTTP_handle_not_found()
{
    HTTP_server.send(404, "text/plain", "Not found, chummer. Try another page.");
}

void ESP_HTTP_handle_ctrl()
{
    char cmd_1[STR_MAX_LEN + 1] = CMD_1;
    cmd_1[strlen(cmd_1) - 1] = '\0';

    char cmd_2[STR_MAX_LEN + 1] = CMD_2;
    cmd_2[strlen(cmd_2) - 1] = '\0';

    if (HTTP_server.hasArg(cmd_1)) {
        if (HTTP_server.arg(cmd_1).length() > STR_MAX_LEN) {
            HTTP_server.send(200, "text/plain", "Command buffer overflow.");
            return;
        }

        char val[STR_MAX_LEN + 1] = {0};
        strcpy(val, HTTP_server.arg(cmd_1).c_str());

        uint32_t prev = 0;
        if (!strcmp(val, "ON")) {
            prev = PREVIOUS_CMD_ON;
        } else if (!strcmp(val, "OFF")) {
            prev = PREVIOUS_CMD_OFF;
        } else {
            HTTP_server.send(200, "text/plain", "No valid value submitted.");
            return;
        }

        strcpy(ESP_HTTP_buf, CMD_PREFIX);
        strcat(ESP_HTTP_buf, CMD_1);
        strcat(ESP_HTTP_buf, val);
        HTTP_server.send(200, "text/html", ESP_HTTP_send_HTML(prev));

        return;
    } else if (HTTP_server.hasArg(cmd_2)) {
        // TODO.
    } else {
        HTTP_server.send(200, "text/plain", "No valid command entered.");
    }
}

String ESP_HTTP_send_HTML(uint32_t previous_cmd)
{
    // Mention of a previous command.
    String prev_style;
    String prev_text;

    if (previous_cmd == PREVIOUS_CMD_ON) {
        prev_style+= "#prev {";
            prev_style+= "color: red;";        // Difference is here.
            prev_style+= "text-align: left;";
            prev_style+= "font-size: 20px;";
        prev_style+= "}";

        prev_text+= "<p id=\"prev\">";
            prev_text+= "Load ON";             // Difference is here.
        prev_text+= "</p>";
    } else if (previous_cmd == PREVIOUS_CMD_OFF) {
        prev_style+= "#prev {";
            prev_style+= "color: blue;";       // Difference is here.
            prev_style+= "text-align: left;";
            prev_style+= "font-size: 20px;";
        prev_style+= "}";

        prev_text+= "<p id=\"prev\">";
            prev_text+= "Load OFF";            // Difference is here.
        prev_text+= "</p>";
    }

    // Basic site.
    String site = "";

    site+= "<!DOCTYPE html>";
    site+= "<html>\n";

        site+= "<head>";
            site+= "<meta name=\"viewport\" content=\"width=device-width\">";
            site+= "<style>";
                if (previous_cmd) site+= prev_style;

                site+= CSS_STYLE_DIV;

                site+= "p {";
                    site+= "margin: 0;";
                site+= "}";

                site+= "select {";
                    site+= "font-size: 30px;";
                    site+= "margin-right: 20px;";
                site+= "}";

                site+= "button {";
                    site+= "font-size: 30px;";
                site+= "}";
            site+= "</style>";
        site+= "</head>";

        site+= "<body>";

            if (previous_cmd) site+= prev_text;

            site+= "<div>";
                site+= "<form action=\"/ctrl\" method=\"POST\">";
                    site+= "<label for=\"LOADDIGITAL\">Issue a command</label>";
                    site+= "<p>";
                        site+= "<select name=\"LOADDIGITAL\" id=\"LOADDIGITAL\">";
                            site+= "<option value=\"" "ON" "\">ON</option>";
                            site+= "<option value=\"" "OFF" "\">OFF</option>";
                        site+= "</select>";
                        site+= "<button type=\"submit\">Send</button>";
                    site+= "</p>";
                site+= "</form>";
            site+= "</div>";
        site+= "</body>";

    site+= "</html>";

    return site;
}

void ESP_HTTP_copy_buf(char *buf, uint32_t str_max_len)
{
    if (ESP_HTTP_buf[0] != '\0' && strlen(ESP_HTTP_buf) <= str_max_len) {
        strcpy(buf, ESP_HTTP_buf);
    }
    ESP_HTTP_buf[0] = '\0';
}
