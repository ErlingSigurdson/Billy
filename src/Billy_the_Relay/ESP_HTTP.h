/****************** DESCRIPTION *****************/

/**
 * Имя файла: ESP_HTTP.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: простой HTTP-сервер для модулей ESP32 и ESP8266.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef ESP_HTTP_H
#define ESP_HTTP_H

// Настройки проекта.
#include "config_general.h"
#include "config_ASCII_cmd_handler.h"


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

void ESP_HTTP_server_start();
void ESP_HTTP_handle_client_in_loop();
void ESP_HTTP_handle_root();
void ESP_HTTP_handle_not_found();
void ESP_HTTP_handle_ctrl();
void ESP_HTTP_copy_value(char *buf, uint32_t str_max_len);
String ESP_HTTP_send_HTML(uint32_t previous_cmd);
void ESP_HTTP_set_handlers();


#endif  // Завершающая директива защиты от повторного включения заголовочного файла.
