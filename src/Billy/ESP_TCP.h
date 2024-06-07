/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_TCP.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: TCP/IP-related functions.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP_TCP_H
#define ESP_TCP_H


/*--- Includes ---*/

// Main Arduino library.
#include <Arduino.h>


/*--- Misc ---*/

#define DEFAULT_IOT_REQ_PERIOD 500


/************** FUNCTION PROTOTYPES *************/

/*--- Local server ---*/

void ESP_TCP_server_port_update(uint32_t port);
void ESP_TCP_server_start();
bool ESP_TCP_server_get_client();
uint32_t ESP_TCP_server_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
void ESP_TCP_server_send_msg(const char *msg);
void ESP_TCP_server_stop(uint32_t shutdown_downtime);


/*--- Local client ---*/

bool ESP_TCP_client_get_server(char *tcp_client_target_IP, uint32_t tcp_client_target_port);
void ESP_TCP_client_send_msg(const char *msg);
uint32_t ESP_TCP_client_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
void ESP_TCP_clients_disconnect(uint32_t shutdown_downtime);


#endif  // Include guards.
