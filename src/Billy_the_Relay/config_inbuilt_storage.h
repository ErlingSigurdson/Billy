/****************** DESCRIPTION *****************/

/**
 * Filename: config_inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: настройки проекта, связанные с работой со встроенной флеш-памятью
 * модуля ESP32 или ESP8266, используемой в качестве накопителя.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef CONFIG_INBUILT_STORAGE_H
#define CONFIG_INBUILT_STORAGE_H


/*--- Основные значения ---*/

#define INBUILT_STORAGE_ITEM_LIST_LEN 11  // Количество хранимых строк.
#define INBUILT_STORAGE_SIZE 1024
#define INBUILT_STORAGE_STR_MAX_LEN 100


/*--- Адреса хранимых настроек ---*/

// Аутентификационные данные для подключения к локальной сети Wi-Fi.
#define INBUILT_STORAGE_ADDR_SSID 0
#define INBUILT_STORAGE_ADDR_PSWD 101

// Настройки локального сервера.
#define INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT 202

// Настройки связи с удалённым сервером (сервером IoT).
#define INBUILT_STORAGE_ADDR_IOT_FLAG 303
#define INBUILT_STORAGE_ADDR_IOT_SERVER_IP 404
#define INBUILT_STORAGE_ADDR_IOT_SERVER_PORT 505
#define INBUILT_STORAGE_ADDR_IOT_REQ_MSG 606
#define INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD 707

// Настройки Bluetooth.
#define INBUILT_STORAGE_ADDR_BT_FLAG 808
#define INBUILT_STORAGE_ADDR_BT_DEV_NAME 909

// Настройки вывода значения RSSI.
#define INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG 1010


/*--- Прочее ---*/

/* Указание на то, что в проекте используется устройство,
 * работа с флеш-памятью которого требует вызова ряда
 * дополнительных методов из библиотеки EEPROM.h.
 */
#define THIS_IS_ESP32_OR_ESP8266_OR_STM32


#endif  // Завершающая директива защиты от повторного включения заголовочного файла.
