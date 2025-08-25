/****************** DESCRIPTION *****************/

/**
 * Filename: config_general.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Project basic configs.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_GENERAL_H
#define CONFIG_GENERAL_H


/*--- Bluetooth Classic ---*/

/* Uncomment the following #define directive if your ESP32 module
 * supports the Bluetooth Classic functionality and you're willing to use it.
 *
 * For ESP32 modules which lack that functionality (e.g. ESP32-S2, ESP32-C3,
 * ESP32-C6) the directive must remain commented out.
 *
 * For ESP8266 it's irrelevant, don't worry about it.
 */
//#define BTCLASSIC_PROVIDED


/*--- Bluetooth Low Energy ---*/

/* Uncomment the following #define directive if your ESP32 module
 * supports the Bluetooth Low Energy functionality and you're willing to use it.
 *
 * For ESP32 modules which lack that functionality (e.g. generic ESP32 modules,
 * ones without ESP32-xx designation) the directive must remain commented out.
 *
 * For ESP8266 it's irrelevant, don't worry about it.
 */
//#define BTLE_PROVIDED


/*--- GPIO ---*/

// Assign load control pins.
#define DIGITAL_OUTPUT_PIN 2
#define PWM_OUTPUT_PIN 0

// Assign indicator LED control pin.
#define WIFI_INDICATOR_LED_PIN 2

/* Choose between an uninverted (load turned ON at a high logic level, default)
 * and an inverted (load turned ON at a low logic level) digital control.
 * The latter is handy if your load is driven by a PNP transistor, for example.
 */
//#define INVERTED_DIGITAL_OUTPUT
#ifndef INVERTED_DIGITAL_OUTPUT
    #define DIGITAL_OUTPUT_LOAD_ON 1
    #define DIGITAL_OUTPUT_LOAD_OFF 0
#else
    #define DIGITAL_OUTPUT_LOAD_ON 0
    #define DIGITAL_OUTPUT_LOAD_OFF 1
#endif


/*--- RGB LED control ---*/

#define RGB_LED
#ifdef RGB_LED
    #define RED_PIN         16
    #define GREEN_PIN       17
    #define BLUE_PIN        18
    #define IS_COMMON_ANODE 1
#endif


/*--- Hardware UART ---*/

// Hardware UART baud rate.
#define HW_UART_BAUD_RATE 115200

/* A pause to ensure that reading from a buffer
 * won't run ahead of writing to it.
 */
#define HW_UART_READ_SLOWDOWN 2

// A tiny pause to allow for an interface startup.
#define HW_UART_STARTUP_PAUSE 100


/*--- Wi-Fi ---*/

#define WIFI_FAILED_CONN_ATTEMPT_PAUSE 1000

// Connection indication.
#define WIFI_INDICATE_CONNECTION_CYCLES 20
#define WIFI_INDICATE_CONNECTION_PERIOD 90

#define WIFI_RSSI_OUTPUT_PERIOD 2000
#define WIFI_RECONNECT_CHECK_PERIOD 30000


/*--- HTTP ---*/

#define HTTP_PORT 80


/*--- Misc ---*/

#define STR_MAX_LEN 100
#define ANTINOISE_PAUSE 1000
#define CONN_TIMEOUT 8000
#define CONN_SHUTDOWN_DOWNTIME 100


#endif  // Include guards.
