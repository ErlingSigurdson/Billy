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
#ifdef ESP32  // This macro is provided by the Arduino framework for the respective SoC family.
    #define BT_CLASSIC_PROVIDED
#endif


/*--- GPIO ---*/

// Assign load control pins.
#define DIGITAL_OUTPUT_PIN 2
#define PWM_OUTPUT_PIN 5

// Assign indicator LED control pin.
#define WIFI_INDICATOR_LED_PIN 2

/* Choose between an uninverted (load turned ON at high logic level, default)
 * and an inverted (load turned ON at low logic level) digital output.
 * The latter is handy if your load is driven by a PNP transistor, for example.
 */
//#define INVERTED_DIGITAL_OUTPUT
#ifndef INVERTED_DIGITAL_OUTPUT
    #define DIGITAL_OUTPUT_LOAD_ON 1
    #define DIGITAL_OUTPUT_LOAD_OFF 0
#endif
#ifdef INVERTED_DIGITAL_OUTPUT
    #define DIGITAL_OUTPUT_LOAD_ON 0
    #define DIGITAL_OUTPUT_LOAD_OFF 1
#endif


/*--- Hardware UART ---*/

// Hardware UART baud rate.
#define HW_UART_BAUD_RATE 115200

/* Pause to ensure that the reading from the buffer
 * won't run ahead of writing to it.
 */
#define HW_UART_READ_SLOWDOWN 2

// Tiny pause to allow interface startup.
#define HW_UART_STARTUP_PAUSE 100


/*--- Wi-Fi ---*/

// Connection indication.
#define WIFI_INDICATE_CONNECTION_CYCLES 20
#define WIFI_INDICATE_CONNECTION_PERIOD 90

#define WIFI_RSSI_OUTPUT_PERIOD 2000


/*--- HTTP ---*/

#define HTTP_PORT 80


/*--- Misc ---*/

#define STR_MAX_LEN 100
#define CONN_TIMEOUT 8000
#define CONN_SHUTDOWN_DOWNTIME 100


#endif  // Include guards.
