/****************** DESCRIPTION *****************/

/**
 * Filename: config_ASCII_cmd_handler.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: text commands configs.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_ASCII_CMD_HANDLER_H
#define CONFIG_ASCII_CMD_HANDLER_H

// Text commands.
#define CMD_PREFIX  "AT+"
#define CMD_0       "SETLOAD="          // Turn load ON or OFF. Main workhorse.

#define CMD_1       "SETLOCALSSID="     // Change an SSID of a local Wi-Fi access point  stored in the inbuilt storage.

#define CMD_2       "PRINTLOCALSSID"    // Print an SSID of a local Wi-Fi access point  stored in the inbuilt storage.

#define CMD_3       "SETLOCALPSWD="     /* Change a password for a local Wi-Fi access point
                                         * stored in the inbuilt storage.
                                         */

#define CMD_4       "SETLOCALPORT="     // Change a local TCP server port number stored in the inbuilt storage.      

#define CMD_5       "PRINTLOCALPORT"    // Print a local TCP server port number stored in the inbuilt storage.

#define CMD_6       "PRINTLOCALIP"      // Print local TCP server current local IP.

#define CMD_7       "RSTLOCALCONN"      // Reset local connections.

#define CMD_8       "SETIOT="           // Set IoT mode (attempts to connect to a remote server) ON or OFF.

#define CMD_9       "SETIOTIP="         // Change an IP address of a remote server stored in the inbuilt storage.

#define CMD_10      "PRINTIOTIP"        // Print an IP address of a remote server stored in the inbuilt storage.

#define CMD_11      "SETIOTPORT="       /* Change port number used for sending requests to a remote server
                                         * stored in the inbuilt storage.
                                         */

#define CMD_12      "PRINTIOTPORT"      /* Print port number used for sending requests to a remote server
                                         * stored in the inbuilt storage.
                                         */

#define CMD_13      "SETIOTREQMSG="     /* Change request message to be sent to a remote server
                                         * stored in the inbuilt storage.
                                         */

#define CMD_14      "PRINTIOTREQMSG"    /* Print request message to be sent to a remote server
                                         * stored in the inbuilt storage.
                                         */

#define CMD_15      "SETIOTREQPERIOD="  /* Change the interval (in ms) for sending requests to a remote server
                                         * stored in the inbuilt storage.
                                         */

#define CMD_16      "SETBT="            // Set Bluetooth Classic functionality ON or OFF.

#define CMD_17      "SETBTDEVNAME="     /* Change ESP's name as a Bluetooth slave device
                                         * stored in the inbuilt storage.
                                         */
                                         
#define CMD_18      "PRINTBTDEVNAME"    /* Print ESP's name as a Bluetooth slave device
                                         * stored in the inbuilt storage.
                                         */

#define CMD_19      "SETRSSIPRINT="     // Set periodical printount of a current RSSI value ON or OFF.

// Evaluate command list length.
#define CMD_LIST_LEN (CMD_0_DEFINED + CMD_1_DEFINED + CMD_2_DEFINED + CMD_3_DEFINED + CMD_4_DEFINED         \
                      + CMD_5_DEFINED + CMD_6_DEFINED + CMD_7_DEFINED + CMD_8_DEFINED + CMD_9_DEFINED       \
                      + CMD_10_DEFINED + CMD_11_DEFINED + CMD_12_DEFINED + CMD_13_DEFINED + CMD_14_DEFINED  \
                      + CMD_15_DEFINED + CMD_16_DEFINED + CMD_17_DEFINED + CMD_18_DEFINED + CMD_19_DEFINED)

#ifdef CMD_0
    #define CMD_0_DEFINED 1
#else
    #define CMD_0_DEFINED 0
#endif

#ifdef CMD_1
    #define CMD_1_DEFINED 1
#else
    #define CMD_1_DEFINED 0
#endif

#ifdef CMD_2
    #define CMD_2_DEFINED 1
#else
    #define CMD_2_DEFINED 0
#endif

#ifdef CMD_3
    #define CMD_3_DEFINED 1
#else
    #define CMD_3_DEFINED 0
#endif

#ifdef CMD_4
    #define CMD_4_DEFINED 1
#else
    #define CMD_4_DEFINED 0
#endif

#ifdef CMD_5
    #define CMD_5_DEFINED 1
#else
    #define CMD_5_DEFINED 0
#endif

#ifdef CMD_6
    #define CMD_6_DEFINED 1
#else
    #define CMD_6_DEFINED 0
#endif

#ifdef CMD_7
    #define CMD_7_DEFINED 1
#else
    #define CMD_7_DEFINED 0
#endif

#ifdef CMD_8
    #define CMD_8_DEFINED 1
#else
    #define CMD_8_DEFINED 0
#endif

#ifdef CMD_9
    #define CMD_9_DEFINED 1
#else
    #define CMD_9_DEFINED 0
#endif

#ifdef CMD_10
    #define CMD_10_DEFINED 1
#else
    #define CMD_10_DEFINED 0
#endif

#ifdef CMD_11
    #define CMD_11_DEFINED 1
#else
    #define CMD_11_DEFINED 0
#endif

#ifdef CMD_12
    #define CMD_12_DEFINED 1
#else
    #define CMD_12_DEFINED 0
#endif

#ifdef CMD_13
    #define CMD_13_DEFINED 1
#else
    #define CMD_13_DEFINED 0
#endif

#ifdef CMD_14
    #define CMD_14_DEFINED 1
#else
    #define CMD_14_DEFINED 0
#endif

#ifdef CMD_15
    #define CMD_15_DEFINED 1
#else
    #define CMD_15_DEFINED 0
#endif

#ifdef CMD_16
    #define CMD_16_DEFINED 1
#else
    #define CMD_16_DEFINED 0
#endif

#ifdef CMD_17
    #define CMD_17_DEFINED 1
#else
    #define CMD_17_DEFINED 0
#endif

#ifdef CMD_18
    #define CMD_18_DEFINED 1
#else
    #define CMD_18_DEFINED 0
#endif

#ifdef CMD_19
    #define CMD_19_DEFINED 1
#else
    #define CMD_19_DEFINED 0
#endif


#endif  // Include guards.
