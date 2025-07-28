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


/*--- Drive a 7-segment 4-digit display using 2 daisy-chained 74HC595 ICs ---*/

/* If you wish to control a 7-segment 4-digit display using two daisy-chained 74HC595 ICs,
 * uncomment one of the following #define directives.
 */

// Bit-banging version.
#define DRV7SEG4D2X595_BIT_BANGING

// SPI version, custom MOSI and SCK pins.
//#define DRV7SEG4D2X595_SPI_CUSTOM_PINS

// SPI version, default MOSI and SCK pins.
//#define DRV7SEG4D2X595_SPI_DEFAULT_PINS

// These directives make sure that only one variant will be enabled.
#if defined DRV7SEG4D2X595_BIT_BANGING
    #undef DRV7SEG4D2X595_SPI_CUSTOM_PINS
#endif

#if defined DRV7SEG4D2X595_BIT_BANGING || defined DRV7SEG4D2X595_SPI_CUSTOM_PINS
    #undef DRV7SEG4D2X595_SPI_DEFAULT_PINS
#endif

#ifdef DRV7SEG4D2X595_BIT_BANGING
    #define DRV7SEG4D2X595_SEG_STR                   "ED@CGAFB"  /* The string that must reflect the order
                                                                  * of the connections made between the parallel outputs
                                                                  * of a 74HC595 IC and the segment control pins
                                                                  * of a 7-segment display.
                                                                  */
    #define DRV7SEG4D2X595_DATA_PIN                  16
    #define DRV7SEG4D2X595_LATCH_PIN                 27
    #define DRV7SEG4D2X595_CLOCK_PIN                 14
    #define DRV7SEG4D2X595_D1                        7           // The bit that controls the 1st digit of a display.
    #define DRV7SEG4D2X595_D2                        5           // The bit that controls the 2nd digit of a display.
    #define DRV7SEG4D2X595_D3                        3           // The bit that controls the 3rd digit of a display.
    #define DRV7SEG4D2X595_D4                        1           // The bit that controls the 4th digit of a display.
    #define DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY 4
#endif

#ifdef DRV7SEG4D2X595_SPI_CUSTOM_PINS
    #define DRV7SEG4D2X595_SEG_STR                   "ED@CGAFB"  /* The string that must reflect the order
                                                                  * of the connections made between the parallel outputs
                                                                  * of a 74HC595 IC and the segment control pins
                                                                  * of a 7-segment display.
                                                                  */
    #define DRV7SEG4D2X595_MOSI_PIN                  23
    #define DRV7SEG4D2X595_LATCH_PIN                 27
    #define DRV7SEG4D2X595_SCK_PIN                   19
    #define DRV7SEG4D2X595_D1                        7           // The bit that controls the 1st digit of a display.
    #define DRV7SEG4D2X595_D2                        5           // The bit that controls the 2nd digit of a display.
    #define DRV7SEG4D2X595_D3                        3           // The bit that controls the 3rd digit of a display.
    #define DRV7SEG4D2X595_D4                        1           // The bit that controls the 4th digit of a display.
    #define DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY 4
#endif

#ifdef DRV7SEG4D2X595_SPI_DEFAULT_PINS
    #define DRV7SEG4D2X595_SEG_STR                   "ED@CGAFB"  /* The string that must reflect the order
                                                                  * of the connections made between the parallel outputs
                                                                  * of a 74HC595 IC and the segment control pins
                                                                  * of a 7-segment display.
                                                                  */
    #define DRV7SEG4D2X595_LATCH_PIN                 27
    #define DRV7SEG4D2X595_D1                        7           // The bit that controls the 1st digit of a display.
    #define DRV7SEG4D2X595_D2                        5           // The bit that controls the 2nd digit of a display.
    #define DRV7SEG4D2X595_D3                        3           // The bit that controls the 3rd digit of a display.
    #define DRV7SEG4D2X595_D4                        1           // The bit that controls the 4th digit of a display.
    #define DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY 4
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
