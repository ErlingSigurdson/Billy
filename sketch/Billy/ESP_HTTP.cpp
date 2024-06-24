/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_HTTP.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: a simplistic HTTP-server for ESP32/ESP8266.
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


/*--- Misc ---*/

// Modifying the web page according to a previous command.
#define NO_PREVIOUS_CMD  0
#define PREVIOUS_CMD_ON  1
#define PREVIOUS_CMD_OFF 2


/*************** GLOBAL VARIABLES ***************/

#ifdef ESP32
    WebServer HTTP_server(HTTP_PORT);
#endif
#ifdef ESP8266
    ESP8266WebServer HTTP_server(HTTP_PORT);
#endif

char ESP_HTTP_current_val[STR_MAX_LEN + 1] = {0};


/******************* FUNCTIONS ******************/

void ESP_HTTP_server_start()
{
    HTTP_server.begin();
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
    if (HTTP_server.hasArg("AT")) {
        String Str_cmd_buf = HTTP_server.arg("AT");

        if (strlen(Str_cmd_buf.c_str()) > STR_MAX_LEN) {
            HTTP_server.send(200, "text/plain", "Command buffer overflow.");

            return;
        }

        char cmd_buf[STR_MAX_LEN + 1] = CMD_PREFIX;
        strcat(cmd_buf, Str_cmd_buf.c_str());

        // If a command immediately follows the prefix.
        if (strstr(cmd_buf, CMD_1) == cmd_buf + strlen(CMD_PREFIX)) {
            if (strstr(cmd_buf, "=") == NULL) {
                HTTP_server.send(200, "text/plain", "No valid command issued.");

                return;
            }

            char *cmd_val = strstr(cmd_buf, "=") + 1;

            if (*cmd_val == '\0') {
                HTTP_server.send(200, "text/plain", "No valid value submitted.");

                return;
            }

            if (!strcmp(cmd_val, "ON")) {
                HTTP_server.send(200, "text/html", ESP_HTTP_send_HTML(PREVIOUS_CMD_ON));
                strcpy(ESP_HTTP_current_val, cmd_buf);

                return;
            }

            if (!strcmp(cmd_val, "OFF")) {
                HTTP_server.send(200, "text/html", ESP_HTTP_send_HTML(PREVIOUS_CMD_OFF));
                strcpy(ESP_HTTP_current_val, cmd_buf);

                return;
            }
        }
    }

    HTTP_server.send(200, "text/plain", "No valid command entered.");
}

void ESP_HTTP_copy_value(char *buf, uint32_t str_max_len)
{
    if (ESP_HTTP_current_val[0] != '\0' && strlen(ESP_HTTP_current_val) <= str_max_len) {
        strcpy(buf, ESP_HTTP_current_val);
    }
    ESP_HTTP_current_val[0] = '\0';
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

                site+= "div {";
                    site+= "width: 500px;";
                    site+= "height: 300px;";
                    site+= "margin: 0 auto;";
                    site+= "margin-top: 40px;";
                    site+= "text-align: center;";
                    site+= "font-size: 50px;";
                site+= "}";

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
                    site+= "<label for=\"AT\">Issue a command</label>";
                    site+= "<p>";
                        site+= "<select name=\"AT\" id=\"AT\">";
                            site+= "<option value=\"" "LOADDIGITAL=ON" "\">ON</option>";
                            site+= "<option value=\"" "LOADDIGITAL=OFF" "\">OFF</option>";
                        site+= "</select>";
                        site+= "<button type=\"submit\">Send</button>";
                    site+= "</p>";
                site+= "</form>";
            site+= "</div>";
        site+= "</body>";

    site+= "</html>";

    return site;
}

void ESP_HTTP_set_handlers()
{
    // Assigning HTTP request handler functions to particular URIs.
    HTTP_server.on("/", ESP_HTTP_handle_root);
    HTTP_server.onNotFound(ESP_HTTP_handle_not_found);
    HTTP_server.on("/ctrl", ESP_HTTP_handle_ctrl);
}
