/****************** DESCRIPTION *****************/

/**
 * Filename: config_cmd.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: text commands.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_CMD_H
#define CONFIG_CMD_H

// Text commands.
#define CMD_PREFIX  "AT+"

#define CMD_1       "DLOAD="             /* Turn a digital (two-state) load ON or OFF. A main workhorse.
                                          * A prescribed state of the load is NOT stored in the inbuilt storage.
                                          */

#define CMD_2       "ALOAD="             /* Drive a load using PWM. Another main workhorse.
                                          * Valid values of a duty cycle are 0 to 255.
                                          * A prescribed duty cycle value is NOT stored in the inbuilt storage.
                                          */

#define CMD_3       "DLOAD?"             // Print and send to a client the current state of a digital (two-state) load.

#define CMD_4       "LOCALSSID="         /* Change an SSID of a local Wi-Fi access point
                                          * stored in the inbuilt storage.
                                          */

#define CMD_5       "LOCALSSID?"         /* Print and send to a client an SSID of a local Wi-Fi access point
                                          * stored in the inbuilt storage.
                                          */

#define CMD_6       "LOCALPSWD="         /* Change a password for a local Wi-Fi access point
                                          * stored in the inbuilt storage.
                                          */

#define CMD_7       "LOCALPORT="         /* Change a local TCP server port number
                                          * stored in the inbuilt storage.
                                          */

#define CMD_8       "LOCALPORT?"         /* Print and send to a client a local TCP server port number stored
                                          * in the inbuilt storage.
                                          */

#define CMD_9       "LOCALIP?"           // Print and send to a client the device's IP in a local Wi-Fi network.

#define CMD_10      "LOCALCONNRST"       // Reset local connections.

#define CMD_11      "IOT="

#define CMD_12      "IOTIP="

#define CMD_13      "IOTIP?"

#define CMD_14      "IOTPORT="

#define CMD_15      "IOTPORT?"

#define CMD_16      "IOTREQMSG="

#define CMD_17      "IOTREQMSG?"

#define CMD_18      "IOTREQPERIOD="

#define CMD_19      "BTCLASSIC="

#define CMD_20      "BTCLASSICDEVNAME="

#define CMD_21      "BTCLASSICDEVNAME?"

#define CMD_22      "RSSIOUTPUT="


/*--- Сommand list length evaluation ---*/

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

#ifdef CMD_20
    #define CMD_20_DEFINED 1
#else
    #define CMD_20_DEFINED 0
#endif

#ifdef CMD_21
    #define CMD_21_DEFINED 1
#else
    #define CMD_21_DEFINED 0
#endif

#ifdef CMD_22
    #define CMD_22_DEFINED 1
#else
    #define CMD_22_DEFINED 0
#endif

#define CMD_LIST_LEN (CMD_1_DEFINED + CMD_2_DEFINED + CMD_3_DEFINED + CMD_4_DEFINED + CMD_5_DEFINED +       \
                      CMD_6_DEFINED + CMD_7_DEFINED + CMD_8_DEFINED + CMD_9_DEFINED + CMD_10_DEFINED +      \
                      CMD_11_DEFINED + CMD_12_DEFINED + CMD_13_DEFINED + CMD_14_DEFINED + CMD_15_DEFINED +  \
                      CMD_16_DEFINED + CMD_17_DEFINED + CMD_18_DEFINED + CMD_19_DEFINED + CMD_20_DEFINED +  \
                      CMD_21_DEFINED + CMD_22_DEFINED)


#endif  // Include guards.
