/*************** FILE DESCRIPTION ***************/

/**
 * Filename: Billy.ino
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  The main file of Billy, an Arduino sketch written for ESP32
 *           and ESP8266 systems-on-chip (SoCs).
 *
 *           Billy receives commands over UART and wireless connections
 *           and executes them. Most commonly, Billy provides control over
 *           an SoC's outputs.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    See the README files for more information.
 *           Project homepages: https://github.com/ErlingSigurdson/Billy
 *                              https://gitflic.ru/efimov-d-v/billy
 *
 *           Some header and source files use a *_billy.h / *_billy.cpp suffix
 *           to avoid possible naming conflicts with ESP-IDF library files.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Local modules.
#include "cmd.h"
#include "hw_uart.h"
#include "esp_wifi_billy.h"
#include "esp_tcp_billy.h"
#include "esp_http_billy.h"
#include "inbuilt_storage.h"
#include "stored_configs.h"
#include "cstring_utils.h"

#if defined ESP32 && defined BTCLASSIC_USED
    #include "esp32_btclassic_billy.h"
#endif

#if defined ESP32 && defined BLE_USED
    #include "esp32_ble_billy.h"
#endif

#ifdef RGB_LED
    #include "rgb_led.h"
#endif

#if defined DRV7SEG4D2X595_BIT_BANGING      || \
    defined DRV7SEG4D2X595_SPI_CUSTOM_PINS  || \
    defined DRV7SEG4D2X595_SPI_DEFAULT_PINS
    #include "SegMap595.h"
    #include "Drv7Seg4D2x595.h"
    #include "SimpleCounter.h"
#endif

// Project configs.
#include "config_general.h"
#include "config_cmd.h"
#include "config_inbuilt_storage.h"

// Arduino.h, the Arduino core header file, is already included by default.

// Additional Arduino libraries are included in the local modules.

/* In other project files the include order is as follows (irrelevant categories are omitted):
 * - This source file's own header file (for *.cpp files only).
 * - Local modules.
 * - Project configs.
 * - Arduino core header file.
 * - Additional Arduino libraries.
 * - Relevant standard libraries.
 */


/************** FUNCTION PROTOTYPES *************/

/*--- Wireless connectivity setup functions ---*/

namespace wireless_interface_setup {
    bool WiFi(stored_configs_t *stored_configs, uint32_t conn_attempt_timeout);
    void BTClassic(stored_configs_t *stored_configs);
}


/*--- Command reception functions ---*/

namespace receive {
    // Put a data received by hardware UART into the buffer.
    void hw_uart(char *buf);

    // Put a data received by a local TCP server from a client into the buffer.
    void tcp_local(char *buf);

    /* A local TCP client sends a request to a remote server and reads a response,
     * which is then put into the buffer.
     */
    void tcp_iot(char *buf, stored_configs_t *stored_configs);

    /* A local HTTP server handles HTTP requests and reads a data
     * from a request body, which is then put into the buffer.
     */
    void http(char *buf);

    // Put a data received by a Bluetooth Classic slave device from a master device into the buffer.
    void btclassic(char *buf, stored_configs_t *stored_configs, bool *BTClassic_was_connected);
}


/******************* FUNCTIONS ******************/

/*--- Basic functions ---*/

void setup()
{
    /*--- Hardware UART startup ---*/

    hw_uart::setup(HW_UART_BAUD_RATE, HW_UART_ANTINOISE_PAUSE, HW_UART_STARTUP_PAUSE, HW_UART_STARTUP_MSG);


    /*--- Pin setup ---*/

    // Check the assigned pins.
    if (DIGITAL_OUTPUT_PIN <= 0 && PWM_OUTPUT_PIN <= 0) {
        Serial.println("");
        Serial.println("Warning! No output pins specified.");
    } else if ((DIGITAL_OUTPUT_PIN > 0 || PWM_OUTPUT_PIN > 0) && (DIGITAL_OUTPUT_PIN == PWM_OUTPUT_PIN)) {
        Serial.println("");
        Serial.println("Warning! Digital and PWM output pins coincide.");
        Serial.println("The digital (two-state) control will be most probably rendered inoperable.");
        Serial.println("It is advised to reupload the sketch with separate pin numbers specified");
        Serial.println("for the digital and the PWM outputs.");
    }

    if (WIFI_INDICATOR_LED_PIN <= 0) {
        Serial.println("");
        Serial.println("Warning! No Wi-Fi indicator LED output pin specified.");
    }

    #if defined DRV7SEG4D2X595_BIT_BANGING
        #if DRV7SEG4D2X595_DATA_PIN  == DIGITAL_OUTPUT_PIN     || \
            DRV7SEG4D2X595_DATA_PIN  == PWM_OUTPUT_PIN         || \
            DRV7SEG4D2X595_DATA_PIN  == WIFI_INDICATOR_LED_PIN || \
            DRV7SEG4D2X595_LATCH_PIN == DIGITAL_OUTPUT_PIN     || \
            DRV7SEG4D2X595_LATCH_PIN == PWM_OUTPUT_PIN         || \
            DRV7SEG4D2X595_LATCH_PIN == WIFI_INDICATOR_LED_PIN || \
            DRV7SEG4D2X595_CLOCK_PIN == DIGITAL_OUTPUT_PIN     || \
            DRV7SEG4D2X595_CLOCK_PIN == PWM_OUTPUT_PIN         || \
            DRV7SEG4D2X595_CLOCK_PIN == WIFI_INDICATOR_LED_PIN

            Serial.println("");
            Serial.println("Warning! One or more of the pins assigned to control the daisy-chained 74HC595 ICs");
            Serial.println("coincide with either a digital output pin, a PWM output pin");
            Serial.println("or a Wi-Fi indicator LED control pin.");
            Serial.println("It can, and most probably will, interfere with the output.");
        #endif
    #endif

    #if defined DRV7SEG4D2X595_SPI_CUSTOM_PINS
        #if DRV7SEG4D2X595_MOSI_PIN  == DIGITAL_OUTPUT_PIN     || \
            DRV7SEG4D2X595_MOSI_PIN  == PWM_OUTPUT_PIN         || \
            DRV7SEG4D2X595_MOSI_PIN  == WIFI_INDICATOR_LED_PIN || \
            DRV7SEG4D2X595_LATCH_PIN == DIGITAL_OUTPUT_PIN     || \
            DRV7SEG4D2X595_LATCH_PIN == PWM_OUTPUT_PIN         || \
            DRV7SEG4D2X595_LATCH_PIN == WIFI_INDICATOR_LED_PIN || \
            DRV7SEG4D2X595_SCK_PIN   == DIGITAL_OUTPUT_PIN     || \
            DRV7SEG4D2X595_SCK_PIN   == PWM_OUTPUT_PIN         || \
            DRV7SEG4D2X595_SCK_PIN   == WIFI_INDICATOR_LED_PIN

            Serial.println("");
            Serial.println("Warning! One or more of the pins assigned to control the daisy-chained 74HC595 ICs");
            Serial.println("coincide with either a digital output pin, a PWM output pin");
            Serial.println("or a Wi-Fi indicator LED control pin.");
            Serial.println("It can, and most probably will, interfere with the output.");
        #endif
    #endif

    #if defined DRV7SEG4D2X595_SPI_DEFAULT_PINS
        #if DRV7SEG4D2X595_LATCH_PIN  == DIGITAL_OUTPUT_PIN     || \
            DRV7SEG4D2X595_LATCH_PIN  == PWM_OUTPUT_PIN         || \
            DRV7SEG4D2X595_LATCH_PIN  == WIFI_INDICATOR_LED_PIN

            Serial.println("");
            Serial.println("Warning! The latch pin assigned to control the daisy-chained 74HC595 ICs");
            Serial.println("coincide with either a digital output pin, a PWM output pin");
            Serial.println("or a Wi-Fi indicator LED control pin.");
            Serial.println("It can, and most probably will, interfere with the output.");
        #endif
    #endif

    #if defined DRV7SEG4D2X595_BIT_BANGING
        #if DRV7SEG4D2X595_DATA_PIN  == DRV7SEG4D2X595_LATCH_PIN || \
            DRV7SEG4D2X595_DATA_PIN  == DRV7SEG4D2X595_CLOCK_PIN || \
            DRV7SEG4D2X595_LATCH_PIN == DRV7SEG4D2X595_CLOCK_PIN

            Serial.println("");
            Serial.println("Warning! Some of the pins assigned to control the daisy-chained 74HC595 ICs"
                           "coincide with each other. It can, and most probably will, interfere with the output.");
        #endif
    #endif

    #if defined DRV7SEG4D2X595_SPI_CUSTOM_PINS
        #if DRV7SEG4D2X595_MOSI_PIN  == DRV7SEG4D2X595_LATCH_PIN || \
            DRV7SEG4D2X595_MOSI_PIN  == DRV7SEG4D2X595_SCK_PIN   || \
            DRV7SEG4D2X595_LATCH_PIN == DRV7SEG4D2X595_SCK_PIN

            Serial.println("");
            Serial.println("Warning! Some of the pins assigned to control the daisy-chained 74HC595 ICs"
                           "coincide with each other. It can, and most probably will, interfere with the output.");
        #endif
    #endif

    // Pin configuration and setting the digital outputs to respective initial digital levels.
    if (DIGITAL_OUTPUT_PIN > 0) {
        pinMode(DIGITAL_OUTPUT_PIN, OUTPUT);
        digitalWrite(DIGITAL_OUTPUT_PIN, !DIGITAL_OUTPUT_ACTIVE_STATE);
    }

    if (PWM_OUTPUT_PIN > 0) {
        pinMode(PWM_OUTPUT_PIN, OUTPUT);
    }

    if (WIFI_INDICATOR_LED_PIN > 0) {
        pinMode(WIFI_INDICATOR_LED_PIN, OUTPUT);
        digitalWrite(WIFI_INDICATOR_LED_PIN, !DIGITAL_OUTPUT_ACTIVE_STATE);
    }


    /*--- Inbuilt storage setup ---*/

    /* In contrast to AVR-based devices, ESP32 and ESP8266 SoCs'
     * inbuilt storage must be initialized before use.
     */
    inbuilt_storage::init(INBUILT_STORAGE_SIZE);

    // Read the stored configs from an inbuilt storage into the struct.
    stored_configs_t stored_configs;
    stored_configs_read(&stored_configs);


    /*--- Wireless connectivity setup ---*/

    wireless_interface_setup::WiFi(&stored_configs, CONN_TIMEOUT);
    wireless_interface_setup::BTClassic(&stored_configs);


    /*--- RGB output initialization ---*/

    #ifdef RGB_LED
        RGB_LED_init(RED_PIN, GREEN_PIN, BLUE_PIN, IS_COMMON_ANODE);
    #endif


    /*--- Initialize the objects that drive a 7-segment 4-digit display using 2 daisy-chained 74HC595 ICs ---*/

    #if defined DRV7SEG4D2X595_BIT_BANGING      || \
        defined DRV7SEG4D2X595_SPI_CUSTOM_PINS  || \
        defined DRV7SEG4D2X595_SPI_DEFAULT_PINS

        SegMap595.init(DRV7SEG4D2X595_SEG_STR);
    #endif

    #ifdef DRV7SEG4D2X595_BIT_BANGING
        driver7seg.init_bb(DRV7SEG4D2X595_DATA_PIN, DRV7SEG4D2X595_LATCH_PIN, DRV7SEG4D2X595_CLOCK_PIN,
                           DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY);
    #endif

    #ifdef DRV7SEG4D2X595_SPI_CUSTOM_PINS
        driver7seg.init_spi(DRV7SEG4D2X595_MOSI_PIN, DRV7SEG4D2X595_LATCH_PIN, DRV7SEG4D2X595_SCK_PIN,
                            DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY);
    #endif

    #ifdef DRV7SEG4D2X595_SPI_DEFAULT_PINS
        driver7seg.init_spi(DRV7SEG4D2X595_LATCH_PIN, DRV7SEG4D2X595_GHOSTING_PREVENTION_DELAY);
    #endif


    /*--- Finishing setup ---*/

    hw_uart::print(" ");
    hw_uart::flush();
}

void loop()
{
    /*--- Interaction with an inbuilt storage ---*/

    static bool time_to_refresh_stored_configs = 1;

    // Read config values from an inbuilt storage into the struct.
    static stored_configs_t stored_configs;
    if (time_to_refresh_stored_configs) {    // Check if an update is necessary.
        stored_configs_read(&stored_configs);
        time_to_refresh_stored_configs = 0;
    }


    /*--- Array of valid commands ---*/

    static const char *cmd_list[] = {
        0,  // A placeholder to bump the first command's index up to 1.
        CMD_1,  CMD_2,  CMD_3,  CMD_4,  CMD_5,
        CMD_6,  CMD_7,  CMD_8,  CMD_9,  CMD_10,
        CMD_11, CMD_12, CMD_13, CMD_14, CMD_15,
        CMD_16, CMD_17, CMD_18, CMD_19, CMD_20,
        CMD_21, CMD_22, CMD_23, CMD_24, CMD_25,
        CMD_26, CMD_27
    };


    /*--- Command reception ---*/

    /* Main command buffer. All strings sent to Billy by an end user,
     * regardless of the interface, end up here. All checks are performed afterwards.
     */
    char main_buf[STR_MAX_LEN + 1] = {0};

    // Command reception subroutines.
    receive::hw_uart(main_buf);
    receive::tcp_local(main_buf);
    receive::tcp_iot(main_buf, &stored_configs);
    receive::http(main_buf);

    /* Another call for the connected() method of the BluetoothSerial class
     * caused an RTOS crash, hence the additional flag was introduced.
     */
    bool BTClassic_was_connected = 0;
    receive::btclassic(main_buf, &stored_configs, &BTClassic_was_connected);


    /*--- Command handling ---*/

    // Essentially it's the central hub of the whole sketch.

    // Check for a non-empty buffer string.
    if (main_buf[0] != '\0' ) {
        // In case there's a CR or LF in the buffer.
        cstring_utils::nullify_first_cr_or_lf(main_buf);

        /* Commands themselves should be all-uppercase, but values
         * (after an equal sign) may contain meaningful lowercase letters.
         */
        cstring_utils::to_uppercase_before_char(main_buf, '=');  

        // Check for valid commands.
        int32_t func_to_call = cmd::match_in_buf(main_buf, CMD_PREFIX, cmd_list, CMD_LIST_LEN);
        switch (func_to_call) {
            case CMD_MATCH_IN_BUF_ERR_PREFIX:
                cmd::handler::err_prefix();
                break;

            case CMD_MATCH_IN_BUF_ERR_CMD:
                cmd::handler::err_cmd();
                break;

            case 1:
                cmd::handler::set_load_digital(main_buf, DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_ACTIVE_STATE);
                break;

            case 2:
                cmd::handler::set_load_PWM(main_buf, PWM_OUTPUT_PIN);
                break;

            case 3:
                cmd::handler::output_load_digital(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_ACTIVE_STATE);
                break;

            case 4:
                cmd::handler::set_WiFi_SSID(main_buf, &time_to_refresh_stored_configs);
                break;

            case 5:
                cmd::handler::output_WiFi_SSID();
                break;

            case 6:
                cmd::handler::set_WiFi_pswd(main_buf, &time_to_refresh_stored_configs);
                break;

            case 7:
                cmd::handler::set_WiFi_RSSI_output_flag(main_buf, &time_to_refresh_stored_configs);
                break;

            case 8:
                cmd::handler::set_WiFi_autoreconnect_flag(main_buf, &time_to_refresh_stored_configs);
                break;

            case 9:
                cmd::handler::output_local_server_IP();
                break;

            case 10:
                cmd::handler::set_local_server_port(main_buf, &time_to_refresh_stored_configs);
                break;

            case 11:
                cmd::handler::output_local_server_port();
                break;

            case 12:
                cmd::handler::set_IoT_flag(main_buf, &time_to_refresh_stored_configs);
                break;

            case 13:
                cmd::handler::set_IoT_server_IP(main_buf, &time_to_refresh_stored_configs);
                break;

            case 14:
                cmd::handler::output_IoT_server_IP();
                break;

            case 15:
                cmd::handler::set_IoT_server_port(main_buf, &time_to_refresh_stored_configs);
                break;

            case 16:
                cmd::handler::output_IoT_server_port();
                break;

            case 17:
                cmd::handler::set_IoT_req_msg(main_buf, &time_to_refresh_stored_configs);
                break;

            case 18:
                cmd::handler::output_IoT_req_msg();
                break;

            case 19:
                cmd::handler::set_IoT_req_period(main_buf, &time_to_refresh_stored_configs);
                break;

            case 20:
                cmd::handler::set_BTClassic_flag(main_buf,
                                                 wireless_interface_setup::BTClassic,
                                                 &time_to_refresh_stored_configs);
                break;

            case 21:
                cmd::handler::set_BTClassic_dev_name(main_buf,
                                                     wireless_interface_setup::BTClassic,
                                                     &time_to_refresh_stored_configs);
                break;

            case 22:
                cmd::handler::output_BTClassic_dev_name();
                break;

            case 23:
                cmd::handler::all_conn_rst(wireless_interface_setup::WiFi,
                                           wireless_interface_setup::BTClassic,
                                           &stored_configs);
                break;

            case 24:
                cmd::handler::output_version();
                break;

            case 25:
                #ifdef RGB_LED
                    cmd::handler::RGB_output_color(main_buf);
                #else
                    cmd::handler::err_cmd();
                #endif
                break;

            case 26:
                #ifdef RGB_LED
                    cmd::handler::RGB_output_on();
                #else
                    cmd::handler::err_cmd();
                #endif
                break;

            case 27:
                #ifdef RGB_LED
                    cmd::handler::RGB_output_off();
                #else
                    cmd::handler::err_cmd();
                #endif
                break;

            default:
                break;  // Do nothing and hail MISRA.
        }
    }


    /*--- Current RSSI output ---*/

    static uint64_t RSSI_output_current_millis = millis();
    static uint64_t RSSI_output_previous_millis = RSSI_output_current_millis;
    bool RSSI_output_due_time = (RSSI_output_current_millis -
                                 RSSI_output_previous_millis) >
                                 WIFI_RSSI_OUTPUT_PERIOD;

    if (stored_configs.WiFi_RSSI_output_flag) {
        if (RSSI_output_due_time) {
            esp_wifi_billy::rssi_output();
            RSSI_output_previous_millis = RSSI_output_current_millis = millis();
        } else {
            RSSI_output_current_millis = millis();
        }
    }

    Serial.flush();


    /*--- Finishing communications ---*/

    // TCP clients disconnection.
    esp_tcp_billy::clients_disconnect(CONN_SHUTDOWN_DOWNTIME);

    // Bluetooth Classic disconnection.
    #if defined ESP32 && defined BTCLASSIC_USED
        if (BTClassic_was_connected) {
            esp32_btclassic_billy::disconnect(CONN_SHUTDOWN_DOWNTIME);
        }
    #endif


    /*--- Wi-Fi autoreconnect ---*/

    static uint64_t WiFi_autoreconnect_current_millis = millis();
    static uint64_t WiFi_autoreconnect_previous_millis = WiFi_autoreconnect_current_millis;
    bool WiFi_autoreconnect_due_time = (WiFi_autoreconnect_current_millis -
                                       WiFi_autoreconnect_previous_millis) >
                                       WIFI_RECONNECT_CHECK_PERIOD;

    // A small trick to ensure a reconnection attempt regardless of ESP-IDF event-based connection check.
    static bool WiFi_connection_attempt_failed = !esp_wifi_billy::is_connected();

    if (stored_configs.WiFi_autoreconnect_flag) {
        if (WiFi_autoreconnect_due_time) {
            if (!esp_wifi_billy::is_connected() || WiFi_connection_attempt_failed) {
                esp_tcp_billy::clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
                esp_tcp_billy::server_stop(CONN_SHUTDOWN_DOWNTIME);
                WiFi_connection_attempt_failed = !wireless_interface_setup::WiFi(&stored_configs, CONN_TIMEOUT);
            }
            WiFi_autoreconnect_previous_millis = WiFi_autoreconnect_current_millis = millis();
        } else {
            WiFi_autoreconnect_current_millis = millis();
        }
    }

    Serial.flush();


    /*--- Drive a 7-segment 4-digit display using 2 daisy-chained 74HC595 ICs ---*/

    #if defined DRV7SEG4D2X595_BIT_BANGING      || \
        defined DRV7SEG4D2X595_SPI_CUSTOM_PINS  || \
        defined DRV7SEG4D2X595_SPI_DEFAULT_PINS

        SimpleCounter.update();

        uint32_t minutes_tens = SimpleCounter.minutes / 10;
        uint32_t minutes_ones = SimpleCounter.minutes % 10;
        uint32_t seconds_tens = SimpleCounter.seconds / 10;
        uint32_t seconds_ones = SimpleCounter.seconds % 10;

        uint8_t digit_1 = SegMap595.mapped_characters[minutes_tens];
        uint8_t digit_2 = SegMap595.mapped_characters[minutes_ones];
        uint8_t digit_3 = SegMap595.mapped_characters[seconds_tens];
        uint8_t digit_4 = SegMap595.mapped_characters[seconds_ones];

        if (SimpleCounter.seconds % 2) {
            uint32_t dot_bit_pos = SegMap595.get_dot_bit_pos();
            uint8_t dot_bit_pos_mask = 1 << dot_bit_pos;
            digit_2 |= dot_bit_pos_mask;
        }

        driver7seg.shift_out((1 << DRV7SEG4D2X595_D1), digit_1);
        driver7seg.shift_out((1 << DRV7SEG4D2X595_D2), digit_2);
        driver7seg.shift_out((1 << DRV7SEG4D2X595_D3), digit_3);
        driver7seg.shift_out((1 << DRV7SEG4D2X595_D4), digit_4);

        // Optional digital load toggling aligned with the timer.
        static bool digital_load_toggle_flag = 0;
        char modifiable_buf[STR_MAX_LEN + 1] = CMD_PREFIX CMD_1 "TOGGLE";
        uint32_t period = 5;  // Toggles every five minutes.
        if (SimpleCounter.minutes % period == 0 && SimpleCounter.seconds == 0) {
            if (digital_load_toggle_flag) {
                cmd::handler::set_load_digital(modifiable_buf,
                                               DIGITAL_OUTPUT_PIN,
                                               DIGITAL_OUTPUT_ACTIVE_STATE);
                digital_load_toggle_flag = 0;
            }
        } else {
            digital_load_toggle_flag = 1;
        }
    #endif
}


/*--- Wireless connectivity setup functions ---*/

bool wireless_interface_setup::WiFi(stored_configs_t *stored_configs, uint32_t conn_attempt_timeout)
{
    Serial.println("");

    /* Initializing certain objects (class instances) requires specifying
     * their parameters compile-time, since the latter are to be passed
     * to a constructor function. However, it's not possible to specify
     * a proper port number for a WiFiServer class object in advance because
     * an assigned port number is to be read from an inbuilt storage.
     * Therefore the object gets initialized with a dummy value and then
     * becomes updated.
     */
    esp_tcp_billy::server_port_update(stored_configs->local_server_port);

    // Connect to Wi-Fi network.
    bool WiFi_connected = esp_wifi_billy::set_connection(stored_configs->WiFi_SSID,
                                                  stored_configs->WiFi_pswd,
                                                  conn_attempt_timeout);

    if (WiFi_connected) {
        esp_wifi_billy::indicate_connection(WIFI_INDICATOR_LED_PIN,
                                     WIFI_INDICATE_CONNECTION_CYCLES,
                                     WIFI_INDICATE_CONNECTION_PERIOD);

        Serial.print("Current local IP address is: ");
        Serial.println(esp_wifi_billy::get_devices_current_ip());

        esp_tcp_billy::server_start();
        Serial.print("Local TCP server started at port ");
        Serial.println(stored_configs->local_server_port);

        esp_http_billy::server_start();
        esp_http_billy::set_handlers();
        Serial.print("Local HTTP server started at port ");
        Serial.println(HTTP_PORT);
    }

    // Check for RSSI output flag.
    Serial.print("RSSI output: ");
    if (stored_configs->WiFi_RSSI_output_flag != 0) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }

    // Check for automatic reconnect attempts flag.
    Serial.print("Automatic Wi-Fi reconnect attempts: ");
    if (stored_configs->WiFi_autoreconnect_flag != 0) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }

    // Check for IoT mode flag.
    Serial.print("Requests to IoT server: ");
    if (stored_configs->IoT_flag != 0) {
        Serial.println("ON");
        Serial.print("Target IoT server IP address: ");
        Serial.println(stored_configs->IoT_server_IP);
        Serial.print("Target IoT server port: ");
        Serial.println(stored_configs->IoT_server_port);
        Serial.print("Request message: ");
        Serial.println(stored_configs->IoT_req_msg);
        Serial.print("Request period (once per ms): ");
        Serial.println(stored_configs->IoT_req_period);
    } else {
        Serial.println("OFF");
    }

    return WiFi_connected;
}

void wireless_interface_setup::BTClassic(stored_configs_t *stored_configs)
{
    // Dummy statements to prevent warnings connected to a conditional compilation (unused parameter).
    (void)stored_configs;

    #if defined ESP32 && defined BTCLASSIC_USED
        Serial.println("");

        // Check for Bluetooth Classic functionality flag.
        Serial.print("Bluetooth Classic: ");
        if (stored_configs->BTClassic_flag != 0) {
            esp32_btclassic_billy::start(stored_configs->BTClassic_dev_name);
            Serial.println("ON");
            Serial.print("Bluetooth Classic device name: ");
            Serial.println(stored_configs->BTClassic_dev_name);
        } else {
            esp32_btclassic_billy::stop(CONN_SHUTDOWN_DOWNTIME);
            Serial.println("OFF");
        }
    #endif
}


/*--- Command reception functions ---*/

void receive::hw_uart(char *buf)
{
    uint32_t HW_UART_bytes_read = hw_uart::read_line(buf,
                                                     STR_MAX_LEN,
                                                     CONN_TIMEOUT,
                                                     HW_UART_READ_SLOWDOWN);
    if (HW_UART_bytes_read > STR_MAX_LEN) {
        buf[0] = '\0';
        cmd::handler::err_len();
    }
}

void receive::tcp_local(char *buf)
{
    if (esp_tcp_billy::server_get_client()) {
        uint32_t TCP_server_bytes_read = esp_tcp_billy::server_read_line(buf,
                                                                  STR_MAX_LEN,
                                                                  CONN_TIMEOUT);
        if (TCP_server_bytes_read > STR_MAX_LEN) {
            buf[0] = '\0';
            cmd::handler::err_len();
        }
    }
}

void receive::tcp_iot(char *buf, stored_configs_t *stored_configs)
{
    if (stored_configs->IoT_req_period == 0) {
        stored_configs->IoT_req_period = IOT_DEFAULT_REQ_PERIOD;  // Divide by zero prevention.
    }

    if (stored_configs->IoT_flag && millis() % stored_configs->IoT_req_period == 0) {
        Serial.print ("Sending request to an IoT server, target IP: ");
        Serial.print(stored_configs->IoT_server_IP);
        Serial.print(", target port: ");
        Serial.println(stored_configs->IoT_server_port);

        if (esp_tcp_billy::client_get_server(stored_configs->IoT_server_IP, stored_configs->IoT_server_port)) {
            Serial.println("Remote server reached.");
            esp_tcp_billy::client_send_msg(stored_configs->IoT_req_msg);

            uint32_t TCP_client_bytes_read = esp_tcp_billy::client_read_line(buf,
                                                                      STR_MAX_LEN,
                                                                      CONN_TIMEOUT);

            if (TCP_client_bytes_read > 0 && TCP_client_bytes_read < STR_MAX_LEN) {
                cstring_utils::nullify_first_cr_or_lf(buf);
                Serial.print("Message received from remote server: ");
                Serial.println(buf);
            }

            if (TCP_client_bytes_read > STR_MAX_LEN) {
                buf[0] = '\0';
                cmd::handler::err_len();
            }
        } else {
            Serial.println("Remote server unavailable.");
        }
    }
}

void receive::http(char *buf)
{
    esp_http_billy::handle_client_in_loop();
    esp_http_billy::copy_buf(buf, STR_MAX_LEN);
}

void receive::btclassic(char *buf, stored_configs_t *stored_configs, bool *BTClassic_was_connected)
{
    // Dummy statements to prevent warnings connected to a conditional compilation (unused parameter).
    (void)buf;
    (void)stored_configs;
    (void)BTClassic_was_connected;

    #if defined ESP32 && defined BTCLASSIC_USED
        *BTClassic_was_connected = false;  // Just in case.
        if (stored_configs->BTClassic_flag && esp32_btclassic_billy::check_connection()) {
            *BTClassic_was_connected = true;
            uint32_t BTClassic_bytes_read = esp32_btclassic_billy::read_line(buf,
                                                                      STR_MAX_LEN,
                                                                      CONN_TIMEOUT,
                                                                      BTCLASSIC_READ_SLOWDOWN);

            if (BTClassic_bytes_read > STR_MAX_LEN) {
                buf[0] = '\0';
                cmd::handler::err_len();
            }
        }
    #endif
}
