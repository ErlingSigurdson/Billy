/****************** DESCRIPTION *****************/

/**
 * Filename: config_cmd.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Text commands list and its length evaluation.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_CMD_H
#define CONFIG_CMD_H


/*--- Text commands list ---*/

#define CMD_PREFIX "AT+"

#define CMD_1      "LOADDIGITAL="       // Turn a two-state load ON or OFF. Valid values are ON, OFF and TOGGLE.
#define CMD_2      "LOADPWM="           // Drive a load using PWM. Valid values of a duty cycle are 0 to 255.
#define CMD_3      "LOADDIGITAL?"       // Output current state of a two-state load.
#define CMD_4      "WIFISSID="          /* Change the presumed SSID of a Wi-Fi access point
                                         * your device will try to connect to.
                                         */
#define CMD_5      "WIFISSID?"          /* Output the presumed SSID of a Wi-Fi access point
                                         * your device will try to connect to.
                                         */
#define CMD_6      "WIFIPSWD="          // Change the password used for connecting to a Wi-Fi access point.
#define CMD_7      "WIFIRSSIOUTPUT="    // Turn a periodical printing of a current RSSI value ON or OFF.
#define CMD_8      "WIFIAUTORECONN="    // Turn periodical attempts to reconnect to a Wi-Fi network ON or OFF.
#define CMD_9      "LOCALIP?"           // Output the IP that's been assigned to your device within a Wi-Fi network.
#define CMD_10     "LOCALPORT="         // Change the local TCP server incoming port number.
#define CMD_11     "LOCALPORT?"         // Output the local TCP server incoming port number.
#define CMD_12     "IOT="               // Set the IoT mode (attempts to connect to a remote server) ON or OFF.
#define CMD_13     "IOTIP="             // Change the IP address of a remote server.
#define CMD_14     "IOTIP?"             // Output the IP address of a remote server.
#define CMD_15     "IOTPORT="           // Change the port number presumably used by a remote server.
#define CMD_16     "IOTPORT?"           // Output the port number presumably used by a remote server.
#define CMD_17     "IOTREQMSG="         // Change the request message to be sent to a remote server.
#define CMD_18     "IOTREQMSG?"         // Output the request message to be sent to a remote server.
#define CMD_19     "IOTREQPERIOD="      // Change the interval (in ms) of sending requests to a remote server.
#define CMD_20     "BTCLASSIC="         // Turn the Bluetooth Classic functionality ON or OFF.
#define CMD_21     "BTCLASSICDEVNAME="  // Change the name of the ESP32 as a Bluetooth Classic slave device.
#define CMD_22     "BTCLASSICDEVNAME?"  // Output the name of the ESP32 as a Bluetooth Classic slave device.
#define CMD_23     "ALLCONNRST"         // Reset all wireless connections and restart a wireless connectivity.
#define CMD_24     "VERSION?"           // Output the firmware version, contact details and specified pins information.
#define CMD_25     "RGBCOLOR="          // Simple output of a given RGB color.
#define CMD_26     "RGBON"              // Turn the RGB output ON.
#define CMD_27     "RGBOFF"             // Turn the RGB output OFF.


/*--- Text commands list length evaluation ---*/

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

#ifdef CMD_23
    #define CMD_23_DEFINED 1
#else
    #define CMD_23_DEFINED 0
#endif

#ifdef CMD_24
    #define CMD_24_DEFINED 1
#else
    #define CMD_24_DEFINED 0
#endif

#ifdef CMD_25
    #define CMD_25_DEFINED 1
#else
    #define CMD_25_DEFINED 0
#endif

#ifdef CMD_26
    #define CMD_26_DEFINED 1
#else
    #define CMD_26_DEFINED 0
#endif

#ifdef CMD_27
    #define CMD_27_DEFINED 1
#else
    #define CMD_27_DEFINED 0
#endif

#ifdef CMD_28
    #define CMD_28_DEFINED 1
#else
    #define CMD_28_DEFINED 0
#endif

#ifdef CMD_29
    #define CMD_29_DEFINED 1
#else
    #define CMD_29_DEFINED 0
#endif

#ifdef CMD_30
    #define CMD_30_DEFINED 1
#else
    #define CMD_30_DEFINED 0
#endif

#ifdef CMD_31
    #define CMD_31_DEFINED 1
#else
    #define CMD_31_DEFINED 0
#endif

#ifdef CMD_32
    #define CMD_32_DEFINED 1
#else
    #define CMD_32_DEFINED 0
#endif

#ifdef CMD_33
    #define CMD_33_DEFINED 1
#else
    #define CMD_33_DEFINED 0
#endif

#ifdef CMD_34
    #define CMD_34_DEFINED 1
#else
    #define CMD_34_DEFINED 0
#endif

#ifdef CMD_35
    #define CMD_35_DEFINED 1
#else
    #define CMD_35_DEFINED 0
#endif

#ifdef CMD_36
    #define CMD_36_DEFINED 1
#else
    #define CMD_36_DEFINED 0
#endif

#ifdef CMD_37
    #define CMD_37_DEFINED 1
#else
    #define CMD_37_DEFINED 0
#endif

#ifdef CMD_38
    #define CMD_38_DEFINED 1
#else
    #define CMD_38_DEFINED 0
#endif

#ifdef CMD_39
    #define CMD_39_DEFINED 1
#else
    #define CMD_39_DEFINED 0
#endif

#ifdef CMD_40
    #define CMD_40_DEFINED 1
#else
    #define CMD_40_DEFINED 0
#endif

#define CMD_LIST_LEN (CMD_1_DEFINED + CMD_2_DEFINED + CMD_3_DEFINED + CMD_4_DEFINED + CMD_5_DEFINED + \
                      CMD_6_DEFINED + CMD_7_DEFINED + CMD_8_DEFINED + CMD_9_DEFINED + CMD_10_DEFINED + \
                      CMD_11_DEFINED + CMD_12_DEFINED + CMD_13_DEFINED + CMD_14_DEFINED + CMD_15_DEFINED + \
                      CMD_16_DEFINED + CMD_17_DEFINED + CMD_18_DEFINED + CMD_19_DEFINED + CMD_20_DEFINED + \
                      CMD_21_DEFINED + CMD_22_DEFINED + CMD_23_DEFINED + CMD_24_DEFINED + CMD_25_DEFINED + \
                      CMD_26_DEFINED + CMD_27_DEFINED + CMD_28_DEFINED + CMD_29_DEFINED + CMD_30_DEFINED + \
                      CMD_31_DEFINED + CMD_32_DEFINED + CMD_33_DEFINED + CMD_34_DEFINED + CMD_35_DEFINED + \
                      CMD_36_DEFINED + CMD_37_DEFINED + CMD_38_DEFINED + CMD_39_DEFINED + CMD_40_DEFINED)


#endif  // Include guards.
