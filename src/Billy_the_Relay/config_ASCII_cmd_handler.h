/****************** DESCRIPTION *****************/

/**
 * Имя файла: config_ASCII_cmd_handler.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: настройки проекта, связанные с текстовыми командами.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef CONFIG_ASCII_CMD_HANDLER_H
#define CONFIG_ASCII_CMD_HANDLER_H

// Тексты команд.
#define CMD_PREFIX  "AT+"
#define CMD_0       "SETLOAD="          // Включение или выключение нагрузки. Основная "рабочая лошадка".

#define CMD_1       "SETLOCALSSID="     /* Изменение SSID точки доступа локальной сети Wi-Fi, хранящегося
                                         * во встроенном накопителе устройства.
                                         */

#define CMD_2       "PRINTLOCALSSID"    /* Вывод SSID точки доступа локальной сети Wi-Fi, хранящегося во встроенном
                                         * накопителе устройства.
                                         */

#define CMD_3       "SETLOCALPSWD="     /* Изменение пароля точки доступа локальной сети Wi-Fi, хранящегося
                                         * во встроенном накопителе устройства.
                                         */

#define CMD_4       "SETLOCALPORT="     /* Изменение номера порта, используемого устройством как локальным  
                                         * TCP-сервером, хранящегося во встроенном накопителе устройства.
                                         */      

#define CMD_5       "PRINTLOCALPORT"    /* Вывод номера порта, используемого устройством как локальным
                                         * TCP-сервером, хранящегося во встроенном накопителе устройства.
                                         */

#define CMD_6       "PRINTLOCALIP"      // Вывод локального IP-адреса, присвоенного устройству в текущей сети Wi-Fi.
#define CMD_7       "RSTLOCALCONN"      // Перезапуск локальных соединений.
#define CMD_8       "SETIOT="           // Включение или выключение связи с удалённым сервером (сервером IoT).

#define CMD_9       "SETIOTIP="         /* Изменение IP-адреса удалённого сервера, хранящегося во встроенном
                                         * накопителе устройства.
                                         */                                         

#define CMD_10      "PRINTIOTIP"        /* Вывод IP-адреса удалённого сервера, хранящегося во встроенном накопителе
                                         * устройства.
                                         */

#define CMD_11      "SETIOTPORT="       /* Изменение номера порта, в который устройство будет стучаться к удалённому
                                         * серверу, хранящегося во встроенном накопителе устройства.
                                         */

#define CMD_12      "PRINTIOTPORT"      /* Вывод номера порта, в который устройство будет стучаться к удалённому
                                         * серверу, хранящегося во встроенном накопителе устройства.
                                         */

#define CMD_13      "SETIOTREQMSG="     /* Изменение текста запроса, направляемого устройством удалённому серверу,
                                         * хранящегося во встроенном накопителе устройства.
                                         */

#define CMD_14      "PRINTIOTREQMSG"    /* Вывод текста запроса, направляемого устройством удалённому серверу,
                                         * хранящегося во встроенном накопителе устройства.
                                         */

#define CMD_15      "SETIOTREQPERIOD="  /* Изменение периодичности (в миллисекундах) направления запросов удалённому
                                         * серверу, хранящейся во встроенном накопителе устройства.
                                         */

#define CMD_16      "SETBT="            // Включение или выключение Bluetooth.
#define CMD_17      "SETBTDEVNAME="     // Изменение имени устройства как ведомого устройства Bluetooth.
#define CMD_18      "PRINTBTDEVNAME"    // Вывод имени устройства как ведомого устройства Bluetooth.
#define CMD_19      "SETRSSIPRINT="     // Включение или выключение периодического вывода текущего значения RSSI.

// Определение длины списка команд.
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


#endif  // Завершающая директива защиты от повторного включения заголовочного файла.
