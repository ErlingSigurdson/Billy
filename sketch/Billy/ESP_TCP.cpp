/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_TCP.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  TCP/IP wrapper functions.
 *           Written for use with the ESP32/ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Additional libraries for Arduino IDE.
#ifdef ESP32
    #include <WiFi.h>
#endif
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#endif

// Local modules.
#include "ESP_TCP.h"


/*--- Misc ---*/

#define ESP_TCP_PORT_DUMMY 0


/*************** GLOBAL VARIABLES ***************/

WiFiServer *p_TCP_local_server;
WiFiClient TCP_remote_client;

WiFiClient TCP_local_client;


/******************* FUNCTIONS ******************/

/*--- Local server ---*/

void ESP_TCP_server_port_update(uint32_t port)
{
    static WiFiServer TCP_local_server(port);
    p_TCP_local_server = &TCP_local_server;
}

void ESP_TCP_server_start()
{
    p_TCP_local_server->begin();
}

bool ESP_TCP_server_get_client()
{
    TCP_remote_client = p_TCP_local_server->accept();  /* Previously the available() method was used,
                                                        * but nowadays it's deprecated.
                                                        */
    if (TCP_remote_client) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t ESP_TCP_server_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout)
{
    // Connection timeout counter.
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    uint32_t i = 0, j = 0;
    bool lf = 0;
    while (buf[0] == '\0' && current_millis - previous_millis < conn_timeout && !lf) {
        while (TCP_remote_client.available() && current_millis - previous_millis < conn_timeout && !lf) {
            char c = TCP_remote_client.read();
            ++i;

            if (j < str_max_len) {
                buf[j] = c;
                ++j;
            }

            if (c == '\n') {
                lf = 1;
            }

            current_millis = millis();
        }
        current_millis = millis();
    }

    return i;
}

void ESP_TCP_server_send_msg(const char *msg)
{
    if (TCP_remote_client.connected()) {
        TCP_remote_client.println(msg);
    }
}

void ESP_TCP_server_stop(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    p_TCP_local_server->close();
}


/*--- Local client ---*/

bool ESP_TCP_client_get_server(char *target_IP, uint32_t target_port)
{
    if (TCP_local_client.connect(target_IP, target_port)) {
        return 1;
    } else {
        return 0;
    }
}

void ESP_TCP_client_send_msg(const char *msg)
{
    if (TCP_local_client.connected()) {
       TCP_local_client.println(msg);
    }
}

uint32_t ESP_TCP_client_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout)
{
    // Connection timeout counter.
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    uint32_t i = 0, j = 0;
    bool lf = 0;
    while (buf[0] == '\0' && current_millis - previous_millis < conn_timeout && !lf) {
        while (TCP_local_client.available() && current_millis - previous_millis < conn_timeout && !lf) {
            char c = TCP_local_client.read();
            ++i;

            if (j < str_max_len) {
                buf[j] = c;
                ++j;
            }

            if (c == '\n') {
                lf = 1;
            }

            current_millis = millis();
        }
        current_millis = millis();
    }

    return i;
}


/*-- All clients ---*/

void ESP_TCP_clients_disconnect(uint32_t shutdown_downtime)
{
    if (TCP_remote_client.connected()) {
        delay(shutdown_downtime);
        TCP_remote_client.stop();
    }

    if (TCP_local_client.connected()) {
        delay(shutdown_downtime);
        TCP_local_client.stop();
    }
}
