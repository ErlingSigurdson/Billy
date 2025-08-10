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

#ifdef WIFISERVER_INIT_VARIANT_STATIC_ALLOC
bool ESP_TCP_server_port_update(uint32_t port)
{
    static uint32_t i = 1;

    if (i == 1) {
	    ++i;
	    static WiFiServer TCP_local_server_1(port);
	    p_TCP_local_server = &TCP_local_server_1;
	    return 1;
    }

    if (i == 2) {
        ++i;
        static WiFiServer TCP_local_server_2(port);
        p_TCP_local_server = &TCP_local_server_2;
        return 1;
    }

    if (i == 3) {
        ++i;
        static WiFiServer TCP_local_server_3(port);
        p_TCP_local_server = &TCP_local_server_3;
        return 1;
    }

    if (i == 4) {
        ++i;
        static WiFiServer TCP_local_server_4(port);
        p_TCP_local_server = &TCP_local_server_4;
        return 1;
    }

    if (i == 5) {
        ++i;
        static WiFiServer TCP_local_server_5(port);
        p_TCP_local_server = &TCP_local_server_5;
        return 1;
    }

    return 0;
}

#elif defined WIFISERVER_INIT_VARIANT_DYNAMIC_ALLOC
bool ESP_TCP_server_port_update(uint32_t port)
{
    delete p_TCP_local_server;
    p_TCP_local_server = new WiFiServer(port);

    if (p_TCP_local_server) {
        return 1;
    } else {
        return 0;
    }
}
#endif

bool ESP_TCP_server_start()
{
    if (p_TCP_local_server) {
        p_TCP_local_server->begin();
        return 1;
    } else {
        return 0;
    }
}

bool ESP_TCP_server_get_client()
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

bool ESP_TCP_server_send_msg(const char *msg)
{
    if (TCP_remote_client.connected()) {
        TCP_remote_client.println(msg);
        return 1;
    } else {
        return 0;
    }
}

bool ESP_TCP_server_stop(uint32_t shutdown_downtime)
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
