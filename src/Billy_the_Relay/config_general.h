/****************** DESCRIPTION *****************/

/**
 * Filename: config_general.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: основные настройки проекта.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef CONFIG_GENERAL_H
#define CONFIG_GENERAL_H


/*--- Bluetooth ---*/

/* Для модулей ESP32 без функционала Bluetooth Classic
 * (например, ESP32-S2 и ESP32-C3) следует закомментировать.
 */
#ifdef ESP32
    #define BT_CLASSIC_PROVIDED
#endif


/*--- Нагрузка ---*/

// Назначение вывода, управляющего нагрузкой (ВКЛ./ВЫКЛ.).
#define LOAD_PIN 13

/* Выбор между неинвертированным (включение нагрузки при высоком уровне,
 * вариант по умолчанию) и инвертированным (включение нагрузки при низком
 * уровне) выходом. Последний удобен, например, для модулей реле, в которых
 * драйвером служит PNP-транзистор.
 */
//#define INVERTED_OUTPUT
#ifndef INVERTED_OUTPUT
    #define LOAD_ON 1
    #define LOAD_OFF 0
#endif
#ifdef INVERTED_OUTPUT
    #define LOAD_ON 0
    #define LOAD_OFF 1
#endif


/*--- Аппаратный UART ---*/

// Скорость передачи данных через аппаратный интерфейс UART.
#define HW_UART_BAUD_RATE 115200

/* Пауза, благодаря которой микроконтроллер не будет читать данные из буфера
 * быстрее, чем они туда поступают. Паузы в 2 мс вполне достаточно.
 */
#define HW_UART_READ_SLOWDOWN 2

// Небольшая пауза, дающая устройству время на запуск интерфейса.
#define HW_UART_STARTUP_PAUSE 100


/*--- Wi-Fi ---*/

// Настройки индикации подключения.
#define WIFI_INDICATOR_LED_PIN 2
#define WIFI_INDICATE_CONNECTION_CYCLES 20
#define WIFI_INDICATE_CONNECTION_PERIOD 90

#define WIFI_RSSI_PRINT_PERIOD 2000


/*--- HTTP ---*/

#define HTTP_PORT 80


/*--- Прочее ---*/

#define STR_MAX_LEN 100
#define CONN_TIMEOUT 8000
#define CONN_SHUTDOWN_DOWNTIME 100


#endif  // Завершающая директива защиты от повторного включения заголовочного файла.
