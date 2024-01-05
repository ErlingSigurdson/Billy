/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_TCP.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: обмен данными по протоколу TCP/IP для модулей ESP32 и ESP8266.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>

// Local modules.
#include "ESP_TCP.h"

// Additional libraries for Arduino IDE.
#ifdef ESP32
    #include <WiFi.h>         // Вариант для ESP32.
#endif
#ifdef ESP8266
    #include <ESP8266WiFi.h>  // Вариант для ESP8266.
#endif


/*--- Misc ---*/

#define ESP_TCP_PORT_DUMMY 0


/*************** GLOBAL VARIABLES ***************/

WiFiServer TCP_local_server(ESP_TCP_PORT_DUMMY);

WiFiClient TCP_remote_client;
WiFiClient TCP_local_client;


/******************* FUNCTIONS ******************/

/*--- Local server ---*/

void ESP_TCP_server_port_update(uint32_t port)
{
    WiFiServer tmp_TCP_local_server(port);
    TCP_local_server = tmp_TCP_local_server;
}

void ESP_TCP_server_start()
{
    TCP_local_server.begin();
}

bool ESP_TCP_server_get_client()
{
    TCP_remote_client = TCP_local_server.accept();  /* Previously available() method was used,
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

    /* Заметка: если во внешнем цикле убрать проверку первой ячейки буфера
     * на нуль-терминатор, то программа будет продолжать слушать байты
     * даже после прекращения их непрерывного потока, пока не истечёт
     * время подключения или не будет прислан LF.
     */

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
    TCP_local_server.close();
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

    /* Заметка: если во внешнем цикле убрать проверку первой ячейки буфера
     * на нуль-терминатор, то программа будет продолжать слушать байты
     * даже после прекращения их непрерывного потока, пока не истечёт
     * время подключения или не будет прислан LF.
     */

    return i;
}

void ESP_TCP_clients_disconnect(uint32_t shutdown_downtime)
{
    if (TCP_remote_client.connected()) {
        delay(shutdown_downtime);
        TCP_remote_client.stop();
        delay(shutdown_downtime);
    }

    if (TCP_local_client.connected()) {
        delay(shutdown_downtime);
        TCP_local_client.stop();
        delay(shutdown_downtime);
    }
}
