/****************** DESCRIPTION *****************/

/**
 * Filename: ESP32_Bluetooth.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: обмен данными через Bluetooth для модулей ESP32.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef ESP32_BLUETOOTH_H
#define ESP32_BLUETOOTH_H

// Настройки проекта.
#include "config_general.h"

// Условная компиляция.
#if defined ESP32 && defined BT_CLASSIC_PROVIDED


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

void ESP32_BT_start(char *dev_name);
bool ESP32_BT_check_connection();
uint32_t ESP32_BT_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
void ESP32_BT_send_msg(const char *msg);
void ESP32_BT_disconnect(uint32_t shutdown_downtime);
void ESP32_BT_stop(uint32_t shutdown_downtime);


#endif  // Завершающая директива условной компиляции.
#endif  // Завершающая директива защиты от повторного включения заголовочного файла.
