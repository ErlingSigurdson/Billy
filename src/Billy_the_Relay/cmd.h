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
//#include <Arduino.h>


/************** FUNCTION PROTOTYPES *************/

/*--- Contents checking functions ---*/

bool cmd_check_prefix(char *buf, const char *prefix);
int32_t cmd_check_body(char *buf, const char *cmd_list[], uint32_t cmd_list_len);


/*--- Handler functions ---*/

/* Functions called in response to received commands
 * and command processing errors. 
 */

// Incorrect/absent command prefix.
void cmd_handler_err_prefix();

// No valid command after the prefix.
void cmd_handler_err_cmd();

// No valid value after the command which requires it.
void cmd_handler_err_val();

// Incoming byte string is too long.
void cmd_handler_err_len();

/* Command #0:
 * turn switchable load ON of OFF. Main workhorse.
 */
void cmd_handler_switch_load(char *cmd);

/* Command #1:
 * drive load using PWM. Main workhorse.
 */
void cmd_handler_PWM_load(char *cmd);

/* Command #2:
 * change an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_set_local_SSID(char *cmd);

/* Command #3
 * print an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_print_local_SSID();

/* Command #4:
 * change a password for a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_set_local_pswd(char *cmd);

/* Command #5:
 * change a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_set_local_port(char *cmd);

/* Command #6:
 * print a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_print_local_port();

/* Command #7:
 * print local TCP server current local IP.
 */
void cmd_handler_print_local_IP();

/* Command #8:
 * reset local connections.
 */
void cmd_handler_rst_local_conn(void (*setup_ptr)(void));

/* Command #9:
 * set IoT mode (attempts to connect to a remote server) ON or OFF.
 */
void cmd_handler_set_IoT_flag(char *cmd);

/* Command #10:
 * change an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_server_IP(char *cmd);

/* Command #11
 * print an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_print_IoT_server_IP();

/* Command #12:
 * change port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_server_port(char *cmd);

/* Command #13:
 * print port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_print_IoT_server_port();

/* Command #14:
 * change request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_req_msg(char *cmd);

/* Command #15:
 * print request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_print_IoT_req_msg();

/* Command #16:
 * change the interval (in ms) for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_req_period(char *cmd);

/* Command #17:
 * set Bluetooth Classic functionality ON or OFF.
 */
void cmd_handler_set_BT_flag(char *cmd, void (*setup_ptr)(void));

/* Command #18:
 * change ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_set_BT_dev_name(char *cmd);

/* Command #19:
 * print ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_print_BT_dev_name();

/* Command #20:
 * set periodical printount of a current RSSI value ON or OFF.
 */
void cmd_handler_set_RSSI_print_flag(char *cmd);


/*--- Auxiliary functions ---*/

// Accessories for handler functions.

void cmd_aux_send(const char *msg);
void cmd_aux_print(const char *topic, uint32_t addr);
void cmd_aux_set_val(char *cmd, const char *topic, uint32_t addr, bool echo);

// Set a pin to a logical level.
void cmd_aux_write_digital(uint8_t pin, uint8_t state, const char *msg);

// Launch PWM with a certain duty cycle.
void cmd_aux_write_analog(uint8_t pin, uint32_t val, const char *msg);
 

#endif  // Include guards.
