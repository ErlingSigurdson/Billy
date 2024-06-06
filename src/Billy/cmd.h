/****************** DESCRIPTION *****************/

/**
 * Filename: cmd.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: command processing.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CMD_H
#define CMD_H


/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>


/*--- Misc ---*/

/* To print or not to print a new value of a changed config.
 * OFF for password, ON for other cases.
 */
#define ECHO_VAL_ON 1
#define ECHO_VAL_OFF 0


/************** FUNCTION PROTOTYPES *************/

/*--- Buffer contents check ---*/

// Look up for a valid prefix and for valid commands from a given list.
int32_t cmd_check(const char *buf, const char *prefix, const char *cmd_list[], uint32_t cmd_list_len);


/*--- Auxiliary functions (helper functions, accessories) ---*/

/* A generic accessory called by the other accessories.
 * Prints a message over the UART and sends it over the wireless connections. 
 */
void cmd_aux_output(const char *msg);

// Accessories for handler functions.
void cmd_aux_set_digital_load(uint8_t pin, uint8_t state, const char *topic);
void cmd_aux_set_analog_load(uint8_t pin, uint32_t val, const char *topic);
void cmd_aux_set_config(const char *cmd, uint32_t addr, const char *topic, bool echo_val, bool *refresh_flag);
void cmd_aux_output_config(uint32_t addr, const char *topic);


/*--- Handler functions ---*/

/* Functions called in a response to received commands
 * and command processing errors.
 */

// Incoming byte string is too long.
void cmd_handler_err_len();

// Invalid/absent command prefix.
void cmd_handler_err_prefix();

// No valid command after a prefix.
void cmd_handler_err_cmd();

// No valid value after a command which requires it.
void cmd_handler_err_val();

/* Command #1:
 * turn a two-state load ON or OFF. A main workhorse.
 * A prescribed state of the load is NOT stored in the inbuilt storage.
 */
void cmd_handler_set_digital_load(const char *cmd);

/* Command #2:
 * drive a load using PWM. Another main workhorse.
 * Valid values of a duty cycle are 0 to 255.
 * A prescribed duty cycle value is NOT stored in the inbuilt storage.
 */
void cmd_handler_set_analog_load(const char *cmd);

/* Command #3:
 * print and send to a client the current state of a two-state load.
 * The actual output is based on a return value of the digitalRead().
 */
void cmd_handler_output_digital_load();

/* Command #4:
 * change an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_set_local_SSID(const char *cmd, bool *refresh_flag);

/* Command #5:
 * print and send to a client an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_output_local_SSID();

/* Command #6:
 * change a password for a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_set_local_pswd(const char *cmd, bool *refresh_flag);

/* Command #7:
 * change a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_set_local_port(const char *cmd, bool *refresh_flag);

/* Command #8:
 * print and send to a client a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_output_local_port();

/* Command #9:
 * print and send to a client the device's IP in a local Wi-Fi network.
 */
void cmd_handler_output_local_IP();

/* Command #10:
 * reset local connections.
 */
void cmd_handler_rst_local_conn(void (*setup_ptr)(void));

/* Command #11:
 * set the IoT mode (attempts to connect to a remote server) ON or OFF.
 */
void cmd_handler_set_IoT_flag(const char *cmd, bool *refresh_flag);

/* Command #12:
 * change an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_server_IP(const char *cmd, bool *refresh_flag);

/* Command #13:
 * print and send to a client an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_server_IP();

/* Command #14:
 * change a port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_server_port(const char *cmd, bool *refresh_flag);

/* Command #15:
 * print and send to a client a port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_server_port();

/* Command #16:
 * change a request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_req_msg(const char *cmd, bool *refresh_flag);

/* Command #17:
 * print and send to a client a request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_req_msg();

/* Command #18:
 * change the interval (in ms) for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_req_period(const char *cmd, bool *refresh_flag);

/* Command #19:
 * set Bluetooth Classic functionality ON or OFF.
 */
void cmd_handler_set_BT_Classic_flag(const char *cmd, void (*setup_ptr)(void), bool *refresh_flag);

/* Command #20:
 * change a name of the ESP as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_set_BT_Classic_dev_name(const char *cmd, bool *refresh_flag);

/* Command #21:
 * print and send to a client a name of the ESP as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_output_BT_Classic_dev_name();

/* Command #22:
 * turn periodical output of a current RSSI value ON or OFF.
 */
void cmd_handler_set_RSSI_print_flag(const char *cmd, bool *refresh_flag);


#endif  // Include guards.
