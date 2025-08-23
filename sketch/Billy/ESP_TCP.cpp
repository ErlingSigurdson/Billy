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


/*************** GLOBAL VARIABLES ***************/

WiFiServer *p_TCP_local_server;
WiFiClient TCP_remote_client;

WiFiClient TCP_local_client;


/******************* FUNCTIONS ******************/

/*--- Local server ---*/

uint32_t ESP_TCP::server_port_update(uint32_t port)
{
    /* Static and dynamic allocation take place in sequence
     * (dynamic allocation is used after the program runs out of predefined static allocations).
     */
    static bool dynamic_alloc = false;

    static uint32_t i = 0;
    ++i;

    if (i == 1) {
	    static WiFiServer TCP_local_server_1(port);
	    p_TCP_local_server = &TCP_local_server_1;
    }

    if (i == 2) {
        static WiFiServer TCP_local_server_2(port);
        p_TCP_local_server = &TCP_local_server_2;
    }

    if (i == 3) {
        static WiFiServer TCP_local_server_3(port);
        p_TCP_local_server = &TCP_local_server_3;
    }

    if (i == 4) {
        static WiFiServer TCP_local_server_4(port);
        p_TCP_local_server = &TCP_local_server_4;
    }

    if (i == 5) {
        static WiFiServer TCP_local_server_5(port);
        p_TCP_local_server = &TCP_local_server_5;
    }

    // The program runs out of predefined static allocations.

    if (i == 6) {
        p_TCP_local_server = new WiFiServer(port);
        dynamic_alloc = true;
    }

    if (i > 6) {
        delete p_TCP_local_server;
        p_TCP_local_server = new WiFiServer(port);
    }

    if (!p_TCP_local_server) {
        return WIFISERVER_ALLOC_NULLPTR;
    } else if (!dynamic_alloc) {
        return WIFISERVER_ALLOC_STATIC;
    } else {
        return WIFISERVER_ALLOC_DYNAMIC;
    }
}

bool ESP_TCP::server_start()
{
    if (p_TCP_local_server) {
        p_TCP_local_server->begin();
        return 1;
    } else {
        return 0;
    }
}

bool ESP_TCP::server_get_client()
{
    if (p_TCP_local_server) {
        // Previously the available() method was used, but nowadays it's deprecated.
        TCP_remote_client = p_TCP_local_server->accept();
    } else {
        return 0;
    }

    if (TCP_remote_client) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t ESP_TCP::server_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout)
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

bool ESP_TCP::server_send_msg(const char *msg)
{
    if (TCP_remote_client.connected()) {
        TCP_remote_client.println(msg);
        return 1;
    } else {
        return 0;
    }
}

bool ESP_TCP::server_stop(uint32_t shutdown_downtime)
{
    if (p_TCP_local_server) {
        delay(shutdown_downtime);
        p_TCP_local_server->close();
        return 1;
    } else {
        return 0;
    }
}


/*--- Local client ---*/

bool ESP_TCP::client_get_server(char *target_IP, uint32_t target_port)
{
    if (TCP_local_client.connect(target_IP, target_port)) {
        return 1;
    } else {
        return 0;
    }
}

void ESP_TCP::client_send_msg(const char *msg)
{
    if (TCP_local_client.connected()) {
       TCP_local_client.println(msg);
    }
}

uint32_t ESP_TCP::client_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout)
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

void ESP_TCP::clients_disconnect(uint32_t shutdown_downtime)
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
