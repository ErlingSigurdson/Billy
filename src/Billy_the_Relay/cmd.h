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


/************** FUNCTION PROTOTYPES *************/

/*--- Contents checking function ---*/

int32_t cmd_check(const char *buf, const char *prefix, const char *cmd_list[], uint32_t cmd_list_len);


/*--- Handler functions ---*/

/* Functions called in a response to received commands
 * and command processing errors. 
 */

// Incoming byte string is too long.
void cmd_handler_err_len();

// Incorrect/absent command prefix.
void cmd_handler_err_prefix();

// No valid command after a prefix.
void cmd_handler_err_cmd();

// No valid value after a command which requires it.
void cmd_handler_err_val();

/* Command #1:
 * turn a two-state load ON or OFF. Main workhorse.
 */
void cmd_handler_set_digital_load(const char *cmd);

/* Command #2:
 * drive a load using PWM. Another main workhorse.
 * Valid values are 0 to 255.
 */
void cmd_handler_set_analog_load(const char *cmd);

/* Command #3:
 * print and send a current state of a two-state load.
 */
void cmd_handler_report_digital_load();

/* Command #4:
 * change an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_update_local_SSID(const char *cmd, bool *refresh_flag);

/* Command #5:
 * print and send an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_output_local_SSID();

/* Command #6:
 * change a password of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_update_local_pswd(const char *cmd, bool *refresh_flag);

/* Command #7:
 * change a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_update_local_port(const char *cmd, bool *refresh_flag);

/* Command #8:
 * print and send a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_output_local_port();

/* Command #9:
 * print and send device's IP in a local network.
 */
void cmd_handler_output_local_IP();

/* Command #10:
 * reset local connections.
 */
void cmd_handler_rst_local_conn(void (*setup_ptr)(void));

/* Command #11:
 * set IoT mode (attempts to connect to a remote server) ON or OFF.
 */
void cmd_handler_update_IoT_flag(const char *cmd, bool *refresh_flag);

/* Command #12:
 * change an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_update_IoT_server_IP(const char *cmd, bool *refresh_flag);

/* Command #13:
 * print and send an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_server_IP();

/* Command #14:
 * change port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_update_IoT_server_port(const char *cmd, bool *refresh_flag);

/* Command #15:
 * print and send a port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_server_port();

/* Command #16:
 * change request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_update_IoT_req_msg(const char *cmd, bool *refresh_flag);

/* Command #17:
 * print and send a request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_req_msg();

/* Command #18:
 * change the interval (in ms) for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_update_IoT_req_period(const char *cmd, bool *refresh_flag);

/* Command #19:
 * set Bluetooth Classic functionality ON or OFF.
 */
void cmd_handler_update_BT_Classic_flag(const char *cmd, void (*setup_ptr)(void), bool *refresh_flag);

/* Command #20:
 * change ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_update_BT_Classic_dev_name(const char *cmd, bool *refresh_flag);

/* Command #21:
 * print and send ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_output_BT_Classic_dev_name();

/* Command #22:
 * turn periodical printount of a current RSSI value ON or OFF.
 */
void cmd_handler_update_RSSI_print_flag(const char *cmd, bool *refresh_flag);


/*--- Auxiliary functions ---*/

// Accessories for handler functions.
void cmd_aux_print_and_send_msg(const char *msg);
void cmd_aux_output_config(const char *topic, uint32_t addr);
void cmd_aux_update_config(const char *cmd, const char *topic, uint32_t addr, bool echo, bool *refresh_flag);
void cmd_aux_set_digital_load(uint8_t pin, uint8_t state, const char *msg);
void cmd_aux_set_analog_load(uint8_t pin, uint32_t val, const char *msg);
 

#endif  // Include guards.
