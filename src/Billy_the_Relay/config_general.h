/****************** DESCRIPTION *****************/

/**
 * Filename: config_general.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: project basic configs.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_GENERAL_H
#define CONFIG_GENERAL_H


/*--- Bluetooth ---*/

/* Comment following #define directive for ESP32 modules
 * which lack Bluetooth Classic functionality (e.g. ESP32-S2 and ESP32-C3).
 * For ESP8266 don't worry about it.
 */
#ifdef ESP32
    #define BT_CLASSIC_PROVIDED
#endif


/*--- Load ---*/

// Assign load control pin.
#define LOAD_PIN 2

/* Choose between uninverted (load turned ON at high logic level, default)
 * and inverted (load turned ON at low logic level) output. The latter is handy
 * if your load is driven by a PNP transistor.
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


/*--- Hardware UART ---*/

// Hardware UART baud rate.
#define HW_UART_BAUD_RATE 115200

/* Pause to ensure that reading from the buffer
 * won't run ahead of writing to it.
 */
#define HW_UART_READ_SLOWDOWN 2

// Tiny pause to allow interface startup.
#define HW_UART_STARTUP_PAUSE 100


/*--- Wi-Fi ---*/

// Connection indication.
#define WIFI_INDICATOR_LED_PIN 2
#define WIFI_INDICATE_CONNECTION_CYCLES 20
#define WIFI_INDICATE_CONNECTION_PERIOD 90

#define WIFI_RSSI_PRINT_PERIOD 2000


/*--- HTTP ---*/

#define HTTP_PORT 80


/*--- Misc ---*/

#define STR_MAX_LEN 100
#define CONN_TIMEOUT 8000
#define CONN_SHUTDOWN_DOWNTIME 100


#endif  // Include guards.
