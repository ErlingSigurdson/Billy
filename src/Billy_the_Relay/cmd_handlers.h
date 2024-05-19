/*************** GLOBAL VARIABLES ***************/

extern bool time_to_refresh_strd_vals;

/*--- Handler functions ---*/

/* Functions called in response to received commands
 * and command processing errors. 
 */

// Incorrect/absent command prefix.
void handle_cmd_err_prefix();

// No command after the prefix.
void handle_cmd_err_cmd();

// No value after the command which requires it.
void handle_cmd_err_val();

// Incoming byte string is too long.
void handle_cmd_err_len();

/* Command #0:
 * turn load ON of OFF. Main workhorse.
 */
void handle_cmd_set_load(char *cmd);

/* Command #1:
 * change an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void handle_cmd_set_local_SSID(char *cmd);

/* Command #2:
 * print an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void handle_cmd_print_local_SSID();

/* Command #3:
 * change a password for a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void handle_cmd_set_local_pswd(char *cmd);

/* Command #4:
 * change a local TCP server port number
 * stored in the inbuilt storage.
 */
void handle_cmd_set_local_port(char *cmd);

/* Command #5:
 * print a local TCP server port number
 * stored in the inbuilt storage.
 */
void handle_cmd_print_local_port();

/* Command #6:
 * print local TCP server current local IP.
 */
void handle_cmd_print_local_IP();

/* Command #7:
 * reset local connections.
 */
void handle_cmd_rst_local_conn(void (*setup_ptr)(void));

/* Command #8:
 * set IoT mode (attempts to connect to a remote server) ON or OFF.
 */
void handle_cmd_set_IoT_flag(char *cmd);

/* Command #9:
 * change an IP address of a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_server_IP(char *cmd);

/* Command #10:
 * print an IP address of a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_print_IoT_server_IP();

/* Command #11:
 * change port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_server_port(char *cmd);

/* Command #12:
 * print port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_print_IoT_server_port();

/* Command #13:
 * change request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_req_msg(char *cmd);

/* Command #14:
 * print request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_print_IoT_req_msg();

/* Command #15:
 * change the interval (in ms) for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_req_period(char *cmd);

/* Command #16:
 * set Bluetooth Classic functionality ON or OFF.
 */
void handle_cmd_set_BT_flag(char *cmd, void (*setup_ptr)(void));

/* Command #17:
 * change ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void handle_cmd_set_BT_dev_name(char *cmd);

/* Command #18:
 * print ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void handle_cmd_print_BT_dev_name();

/* Command #19:
 * set periodical printount of a current RSSI value ON or OFF.
 */
void handle_cmd_set_RSSI_print_flag(char *cmd);


/*--- Handler function subroutines ---*/

// Subroutine for "set" handlers.
void handle_cmd_helper_set(char *cmd, const char *topic, uint32_t addr, bool echo);

// Subroutine for "print" handlers.
void handle_cmd_helper_print(const char *topic, uint32_t addr);

// Subroutine for sending messages.
void handle_cmd_helper_send(const char *msg);
