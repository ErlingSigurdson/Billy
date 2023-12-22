/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: Billy_the_Relay.ino
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: Ардуино-проект на базе модуля ESP32 или ESP8266.
 * Дистанционное управление простой (ВКЛ./ВЫКЛ.) нагрузкой.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Настройки проекта.
#include "config_general.h"            // Помимо файла .ino включается в заголовочные файлы ряда локальных модулей.

#include "config_ASCII_cmd_handler.h"  /* Помимо файла .ino включается в заголовочный файл модуля ESP_HTTP,
                                        * поскольку тому необходимо "знать" тексты команд.
                                        */

#include "config_inbuilt_storage.h"    /* Помимо файла .ino включается в заголовочный файл модуля inbuilt_storage,
                                        * поскольку тому необходимо "знать", что он имеет дело с ESP32/ESP8266.
                                        */

// Локальные модули.
#include "utilities.h"
#include "ASCII_cmd_handler.h"
#include "inbuilt_storage.h"
#include "HW_UART.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"

#if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
    #include "ESP32_Bluetooth.h"
#endif

// Дополнительные библиотеки для Arduino IDE включаются в локальных модулях.


/****************** ТИПЫ ДАННЫХ *****************/

/* Структура, в которую записываются значения настроек,
 * считываемые со встроенного накопителя устройства.
 */
typedef struct strd_vals_t {
    char SSID[STR_MAX_LEN + 1];
    char pswd[STR_MAX_LEN + 1];
    uint32_t local_server_port;
    bool IoT_flag;
    char IoT_server_IP[STR_MAX_LEN + 1];
    uint32_t IoT_server_port;
    char IoT_req_msg[STR_MAX_LEN + 1];
    uint32_t IoT_req_period;
    bool BT_flag;
    char BT_dev_name[STR_MAX_LEN + 1];
    bool RSSI_print_flag;
} strd_vals_t;


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

/*--- Функции-обработчики ---*/

/* Функции, вызываемые в ответ на поступившие команды и на ошибки обработки
 * команд. Ниже указано, при каких условиях вызывается та или иная функция.
 */

// Отсутствует корректный префикс команды.
void handle_cmd_err_prefix();

// После корректного префикса команды отсутствует корректная команда.
void handle_cmd_err_cmd();

/* После корректной команды, требующей ввода значения,
 * отсутствует корректное значение.
 */
void handle_cmd_err_val();

// Длина команды превышает установленный лимит.
void handle_cmd_err_len();

/* Введена команда #0:
 * включение или выключение нагрузки. Основная "рабочая лошадка".
 */
void handle_cmd_set_load(char *cmd);

/* Введена команда #1:
 * изменение SSID точки доступа локальной сети Wi-Fi, хранящегося
 * во встроенном накопителе устройства.
 */
void handle_cmd_set_local_SSID(char *cmd);

/* Введена команда #2:
 * вывод SSID точки доступа локальной сети Wi-Fi, хранящегося
 * во встроенном накопителе устройства.
 */
void handle_cmd_print_local_SSID();

/* Введена команда #3:
 * изменение пароля точки доступа локальной сети Wi-Fi, хранящегося
 * во встроенном накопителе устройства.
 */
void handle_cmd_set_local_pswd(char *cmd);

/* Введена команда #4:
 * изменение номера порта, используемого устройством как локальным TCP-сервером,
 * хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_set_local_port(char *cmd);

/* Введена команда #5:
 * вывод номера порта, используемого устройством как локальным TCP-сервером,
 * хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_print_local_port();

/* Введена команда #6:
 * вывод локального IP-адреса, присвоенного устройству в текущей сети Wi-Fi.
 */
void handle_cmd_print_local_IP();

/* Введена команда #7:
 * перезапуск локальных соединений.
 */
void handle_cmd_rst_local_conn();

/* Введена команда #8:
 * включение или выключение связи с удалённым сервером (сервером IoT).
 */
void handle_cmd_set_IoT_flag(char *cmd);

/* Введена команда #9:
 * изменение IP-адреса удалённого сервера, хранящегося
 * во встроенном накопителе устройства.
 */
void handle_cmd_set_IoT_server_IP(char *cmd);

/* Введена команда #10:
 * вывод IP-адреса удалённого сервера, хранящегося
 * во встроенном накопителе устройства.
 */
void handle_cmd_print_IoT_server_IP();

/* Введена команда #11:
 * изменение номера порта, в который устройство будет стучаться
 * к удалённому серверу, хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_set_IoT_server_port(char *cmd);

/* Введена команда #12:
 * вывод номера порта, в который устройство будет стучаться
 * к удалённому серверу, хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_print_IoT_server_port();

/* Введена команда #13:
 * изменение текста запроса, направляемого устройством удалённому серверу,
 * хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_set_IoT_req_msg(char *cmd);

/* Введена команда #14:
 * вывод текста запроса, направляемого устройством удалённому серверу,
 * хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_print_IoT_req_msg();

/* Введена команда #15:
 * изменение периодичности (в миллисекундах) направления запросов
 * удалённому серверу, хранящейся во встроенном накопителе устройства.
 */
void handle_cmd_set_IoT_req_period(char *cmd);

/* Введена команда #16:
 * включение или выключение Bluetooth.
 */
void handle_cmd_set_BT_flag(char *cmd);

/* Введена команда #17:
 * изменение имени устройства как ведомого устройства Bluetooth,
 * хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_set_BT_dev_name(char *cmd);

/* Введена команда #18:
 * вывод имени устройства как ведомого устройства Bluetooth,
 * хранящегося во встроенном накопителе устройства.
 */
void handle_cmd_print_BT_dev_name();

/* Введена команда #19:
 * включение или выключение периодического вывода текущего значения RSSI.
 */
void handle_cmd_set_RSSI_print_flag(char *cmd);


/*--- Помощники функций-обработчиков ---*/

// Вспомогательная функция для set-обработчиков.
void handle_cmd_helper_set(char *cmd, const char *topic, uint32_t addr, bool echo);

// Вспомогательная функция для print-обработчиков.
void handle_cmd_helper_print(const char *topic, uint32_t addr);

// Вспомогательная функция для отправки сообщений обработчиками.
void handle_cmd_helper_send(const char *msg);


/*--- Прочие функции ---*/

// Чтение настроек со встроенного накопителя устройства в структуру.
void strd_vals_read(strd_vals_t *_strd_vals);


/******************** ФУНКЦИИ *******************/

/*--- Основные функции ---*/

void setup()
{
    /*--- Установка режима работы задействованных выводов устройства ---*/

    pinMode(LOAD_PIN, OUTPUT);
    pinMode(WIFI_INDICATOR_LED_PIN, OUTPUT);


    /*--- Взаимодействие со встроенным накопителем ---*/

    /* У ESP32 и ESP8266, в отличие от устройств на базе микроконтроллеров AVR,
     * встроенный накопитель перед использованием необходимо инициализировать.
     */
    inbuilt_storage_init(INBUILT_STORAGE_SIZE);

    // Чтение настроек со встроенного накопителя устройства в структуру.
    strd_vals_t strd_vals;
    strd_vals_read(&strd_vals);

    /* При необходимости любую настройку можно первоначально сохранить
     * во встроенном накопителе устройства, указав её прямо в коде перед
     * компиляцией. Для этого требуется раскомментировать соответствующие
     * директивы #define и указать желаемые значения среди параметров
     * функции, после чего загрузить скетч в устройство и запустить его.
     * После этого следует вновь закомментировать директивы и повторно
     * загрузить скетч в устройство, поскольку иначе настройки будут
     * сбрасываться к исходным значениям при каждом запуске устройства.
     */

    //#define SET_SSID_AT_UPLOAD
    #ifdef SET_SSID_AT_UPLOAD
        inbuilt_storage_write("NEWSSID",
                              strlen("NEWSSID"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_SSID);
    #endif

    //#define SET_PSWD_AT_UPLOAD
    #ifdef SET_PSWD_AT_UPLOAD
        inbuilt_storage_write("123456",
                              strlen("123456"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_PSWD);
    #endif

    //#define SET_LOCAL_SERVER_PORT_AT_UPLOAD
    #ifdef SET_LOCAL_SERVER_PORT_AT_UPLOAD
        inbuilt_storage_write("451",
                              strlen("451"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT);
    #endif

    //#define SET_IOT_FLAG_AT_UPLOAD
    #ifdef SET_IOT_FLAG_AT_UPLOAD
        inbuilt_storage_write("OFF",
                              strlen("OFF"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_FLAG);
    #endif

    //#define SET_IOT_SERVER_IP_AT_UPLOAD
    #ifdef SET_IOT_SERVER_IP_AT_UPLOAD
        inbuilt_storage_write("111.111.111.111",
                              strlen("111.111.111.111"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_SERVER_IP);
    #endif

    //#define SET_IOT_SERVER_PORT_AT_UPLOAD
    #ifdef SET_IOT_SERVER_PORT_AT_UPLOAD
        inbuilt_storage_write("451",
                              strlen("451"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_SERVER_PORT);
    #endif

    //#define SET_IOT_REQ_MSG_AT_UPLOAD
    #ifdef SET_IOT_REQ_MSG_AT_UPLOAD
        inbuilt_storage_write("UPD_REQ",
                              strlen("UPD_REQ"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_REQ_MSG);
    #endif

    //#define SET_IOT_REQ_PERIOD_AT_UPLOAD
    #ifdef SET_IOT_REQ_PERIOD_AT_UPLOAD
        inbuilt_storage_write("500",
                              strlen("500"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD);
    #endif

    //#define SET_BT_FLAG_AT_UPLOAD
    #ifdef SET_BT_FLAG_AT_UPLOAD
        inbuilt_storage_write("OFF",
                              strlen("OFF"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BT_FLAG);
    #endif

    //#define SET_BT_DEV_NAME_AT_UPLOAD
    #ifdef SET_BT_DEV_NAME_AT_UPLOAD
        inbuilt_storage_write("NEW_BT_DEV_NAME",
                              strlen("NEW_BT_DEV_NAME"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BT_DEV_NAME);
    #endif

    //#define SET_RSSI_PRINT_FLAG_AT_UPLOAD
    #ifdef SET_RSSI_PRINT_FLAG_AT_UPLOAD
        inbuilt_storage_write("OFF",
                              strlen("OFF"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG);
    #endif


    /*--- Запуск коммуникаций ---*/

    // Запуск аппаратного интерфейса UART.
    Serial.begin(HW_UART_BAUD_RATE);
    delay(HW_UART_STARTUP_PAUSE);    // Небольшая пауза, дающая устройству время на запуск интерфейса.

    Serial.println("");
    Serial.println("*** HELLO, HUMAN! ***");

    /* Для инициализации некоторых объектов требуется указать их настройки
     * до компиляции, поскольку те передаются в конструктор в качестве
     * параметров. Чтобы инициализировать такие глобальные объекты в файлах
     * модулей и не тащить их в файл .ino, указываем в качестве параметров
     * конструктора заглушку, а затем уже при выполнении setup() передаём
     * в эти объекты нужные нам значения, считанные со встроенного накопителя.
     */
    ESP_TCP_server_port_update(strd_vals.local_server_port);

    // Подключение к сети Wi-Fi.
    bool WiFi_connected = ESP_WiFi_set_connection(strd_vals.SSID,
                                                  strd_vals.pswd,
                                                  CONN_TIMEOUT);

    if (WiFi_connected) {
        ESP_WiFi_indicate_connection(WIFI_INDICATOR_LED_PIN,
                                     WIFI_INDICATE_CONNECTION_CYCLES,
                                     WIFI_INDICATE_CONNECTION_PERIOD);

        Serial.print("Current local IP address is: ");
        Serial.println(ESP_WiFi_get_current_IP());

        ESP_TCP_server_start();
        Serial.print("Local TCP server started at port ");
        Serial.println(strd_vals.local_server_port);

        ESP_HTTP_server_start();
        ESP_HTTP_set_handlers();
        Serial.print("Local HTTP server started at port ");
        Serial.println(HTTP_PORT);
    }
    Serial.println("");

    // Запуск Bluetooth.
    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        if (strd_vals.BT_flag) {
            ESP32_BT_start(strd_vals.BT_dev_name);
        }
    #endif


    /*--- Вывод значений соответствующих настроек ---*/

    // Проверка флага связи с удалённым сервером (сервером IoT).
    Serial.print("Requests to IoT server: ");
    if (strd_vals.IoT_flag) {
        Serial.println("ON");
        Serial.print("Target IoT server IP address: ");
        Serial.println(strd_vals.IoT_server_IP);
        Serial.print("Target IoT server port: ");
        Serial.println(strd_vals.IoT_server_port);
        Serial.print("Request text: ");
        Serial.println(strd_vals.IoT_req_msg);
        Serial.print("Request period (once per ms): ");
        Serial.println(strd_vals.IoT_req_period);
    } else {
        Serial.println("OFF");
    }

    // Проверка флага работы Bluetooth.
    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        Serial.print("Bluetooth: ");
        if (strd_vals.BT_flag) {
            Serial.println("ON");
            Serial.print("Bluetooth device name: ");
            Serial.println(strd_vals.BT_dev_name);
        } else {
            Serial.println("OFF");
        }
    #endif

    // Проверка флага периодического вывода текущего значения RSSI через UART.
    Serial.print("Print RSSI: ");
    if (strd_vals.RSSI_print_flag) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }
    Serial.println("");
}

void loop()
{
    // Чтение данных со встроенного накопителя в структуру.
    strd_vals_t strd_vals;
    strd_vals_read(&strd_vals);

    // Массив строк с командами.
    static const char *cmd_list[] = {
        CMD_0,
        CMD_1,
        CMD_2,
        CMD_3,
        CMD_4,
        CMD_5,
        CMD_6,
        CMD_7,
        CMD_8,
        CMD_9,
        CMD_10,
        CMD_11,
        CMD_12,
        CMD_13,
        CMD_14,
        CMD_15,
        CMD_16,
        CMD_17,
        CMD_18,
        CMD_19
    };


    /*--- Вывод текущего значения RSSI ---*/
    
    uint32_t WiFi_RSSI_print_period = WIFI_RSSI_PRINT_PERIOD;
    if (WiFi_RSSI_print_period == 0) {
        WiFi_RSSI_print_period = DEFAULT_WIFI_RSSI_PRINT_PERIOD;  // Защита от деления на ноль.
    }

    static bool RSSI_print_loaded = 0;
    if (millis() % WiFi_RSSI_print_period != 0) {
        RSSI_print_loaded = 1;
    }

    if (strd_vals.RSSI_print_flag && millis() % WiFi_RSSI_print_period == 0 && RSSI_print_loaded) {
        ESP_WiFi_RSSI_print();
        RSSI_print_loaded = 0;
    }


    /*--- Приём команд ---*/

    // Буфер для записи команд.
    char terminal_input[STR_MAX_LEN + 1] = {0};

    // Приём данных через аппаратный UART.
    uint32_t HW_UART_bytes_read = HW_UART_read_line(terminal_input,
                                                    STR_MAX_LEN,
                                                    CONN_TIMEOUT,
                                                    HW_UART_READ_SLOWDOWN);
    if (HW_UART_bytes_read > STR_MAX_LEN) {
        terminal_input[0] = '\0';
        handle_cmd_err_len();
    }

    // Приём локальным сервером данных по TCP.
    if (ESP_TCP_server_get_client()) {
        uint32_t TCP_server_bytes_read = ESP_TCP_server_read_line(terminal_input,
                                                                  STR_MAX_LEN,
                                                                  CONN_TIMEOUT);
        if (TCP_server_bytes_read > STR_MAX_LEN) {
            terminal_input[0] = '\0';
            handle_cmd_err_len();
        }
    }

    // Обработка HTTP-запросов и запись значения из тела запроса в буфер.
    ESP_HTTP_handle_client_in_loop();
    ESP_HTTP_copy_value(terminal_input, STR_MAX_LEN);

    /* Направление локальным TCP-клиентом запроса
     * удалённому серверу и приём ответа.
     */
    if (strd_vals.IoT_req_period == 0) {
        strd_vals.IoT_req_period = DEFAULT_IOT_REQ_PERIOD;  // Защита от деления на ноль.
    }

    if (strd_vals.IoT_flag && millis() % strd_vals.IoT_req_period == 0) {
        Serial.print ("Sending request to an IoT server, target IP: ");
        Serial.print(strd_vals.IoT_server_IP);
        Serial.print(", target port: ");
        Serial.println(strd_vals.IoT_server_port);

        if (ESP_TCP_client_get_server(strd_vals.IoT_server_IP, strd_vals.IoT_server_port)) {
            Serial.println("Remote server reached.");
            ESP_TCP_client_send_msg(strd_vals.IoT_req_msg);

            uint32_t TCP_client_bytes_read = ESP_TCP_client_read_line(terminal_input,
                                                                      STR_MAX_LEN,
                                                                      CONN_TIMEOUT);
            if (TCP_client_bytes_read > STR_MAX_LEN) {
                terminal_input[0] = '\0';
                handle_cmd_err_len();
            }
        } else {
            Serial.println("Remote server unavailable.");
        }
    }

    // Приём данных через Bluetooth.
    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        bool BT_was_connected = 0;
        if (strd_vals.BT_flag && ESP32_BT_check_connection()) {
            BT_was_connected = 1;
            uint32_t BT_bytes_read = 0;
            BT_bytes_read = ESP32_BT_read_line(terminal_input, STR_MAX_LEN, CONN_TIMEOUT);
            
            if (BT_bytes_read > STR_MAX_LEN) {
                terminal_input[0] = '\0';
                handle_cmd_err_len();
            }
        }
    #endif


    /*--- Обработка записанных в буфер ASCII-команд ---*/
    
    // По сути это центральный узел всего скетча.

    // Проверка заполненности буфера и наличия префикса в буфере.
    if (terminal_input[0] != '\0' && ASCII_cmd_handler_check_prefix(terminal_input, CMD_PREFIX)) {
        utilities_remove_CR_and_LF(terminal_input);

        // Проверка наличия команд в буфере.
        int32_t func_to_call = ASCII_cmd_handler_check_cmd(terminal_input, cmd_list, CMD_LIST_LEN);
        switch (func_to_call) {
            case -1:
                handle_cmd_err_cmd();
                break;

            case 0:
                handle_cmd_set_load(terminal_input);
                break;

            case 1:
                handle_cmd_set_local_SSID(terminal_input);
                break;

            case 2:
                handle_cmd_print_local_SSID();
                break;

            case 3:
                handle_cmd_set_local_pswd(terminal_input);
                break;

            case 4:
                handle_cmd_set_local_port(terminal_input);
                break;

            case 5:
                handle_cmd_print_local_port();
                break;

            case 6:
                handle_cmd_print_local_IP();
                break;

            case 7:
                handle_cmd_rst_local_conn();
                break;

            case 8:
                handle_cmd_set_IoT_flag(terminal_input);
                break;

            case 9:
                handle_cmd_set_IoT_server_IP(terminal_input);
                break;

            case 10:
                handle_cmd_print_IoT_server_IP();
                break;

            case 11:
                handle_cmd_set_IoT_server_port(terminal_input);
                break;

            case 12:
                handle_cmd_print_IoT_server_port();
                break;

            case 13:
                handle_cmd_set_IoT_req_msg(terminal_input);
                break;

            case 14:
                handle_cmd_print_IoT_req_msg();
                break;

            case 15:
                handle_cmd_set_IoT_req_period(terminal_input);
                break;

            case 16:
                handle_cmd_set_BT_flag(terminal_input);
                break;

            case 17:
                handle_cmd_set_BT_dev_name(terminal_input);
                break;

            case 18:
                handle_cmd_print_BT_dev_name();
                break;

            case 19:
                handle_cmd_set_RSSI_print_flag(terminal_input);
                break;

            default:
                break;  // Ничего не делаем, отдаём дань MISRA.
        }
    } else if (terminal_input[0] != '\0') {
        handle_cmd_err_prefix();
        ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    }
    
    
    /*--- Завершение коммуникации ---*/

    // Отключение TCP-клиентов.
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);

    // Отключение Bluetooth-соединения.
    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        if (BT_was_connected) {  /* Если повторно вызывать метод connected(),
                                  * то крашится RTOS. Поэтому пришлось ввести
                                  * дополнительный флаг.
                                  */
            ESP32_BT_disconnect(CONN_SHUTDOWN_DOWNTIME);
        }
    #endif
}


/*--- Функции-обработчики ---*/

void handle_cmd_err_prefix()
{
    handle_cmd_helper_send("Invalid prefix for a command.");
}

void handle_cmd_err_cmd()
{
    handle_cmd_helper_send("No valid command entered.");
}

void handle_cmd_err_val()
{
    handle_cmd_helper_send("No valid value submitted.");
}

void handle_cmd_err_len()
{
    handle_cmd_helper_send("Command buffer overflow.");
}

// Команда #0
void handle_cmd_set_load(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "TOGGLE")) {
        if (digitalRead(LOAD_PIN) == LOAD_ON) {
            strcpy(cmd_val, "OFF");
        } else {
            strcpy(cmd_val, "ON");
        }
    }

    if (!strcmp(cmd_val, "ON")) {
        digitalWrite(LOAD_PIN, LOAD_ON);
        handle_cmd_helper_send("LOAD ON");

        return;
    }

    if (!strcmp(cmd_val, "OFF")) {
        digitalWrite(LOAD_PIN, LOAD_OFF);
        handle_cmd_helper_send("LOAD OFF");

        return;
    }

    handle_cmd_err_val();
}

/* Выводить или не выводить новое значение изменённой настройки.
 * Для пароля не выводим, для всего остального выводим.
 */
#define ECHO_ON 1
#define ECHO_OFF 0

// Команда #1
void handle_cmd_set_local_SSID(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "SSID changed successfully! New SSID is: ",
                          INBUILT_STORAGE_ADDR_SSID,
                          ECHO_ON);
}

// Команда #2
void handle_cmd_print_local_SSID()
{
    handle_cmd_helper_print("Current SSID is: ",
                            INBUILT_STORAGE_ADDR_SSID);
}

// Команда #3
void handle_cmd_set_local_pswd(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "Password changed successfully!",
                          INBUILT_STORAGE_ADDR_PSWD,
                          ECHO_OFF);
}

// Команда #4
void handle_cmd_set_local_port(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "Local server port changed successfully! New port is: ",
                          INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                          ECHO_ON);

    handle_cmd_helper_send("Please reboot your device or reset local connection to put changes into effect.");
}

// Команда #5
void handle_cmd_print_local_port()
{
    handle_cmd_helper_print("Current local server port is: ",
                            INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT);
}

// Команда #6
void handle_cmd_print_local_IP()
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    String current_IP = ESP_WiFi_get_current_IP();
    strcpy(msg, "Current IP address is: ");
    strcat(msg, current_IP.c_str());
    handle_cmd_helper_send(msg);
}

// Команда #7
void handle_cmd_rst_local_conn()
{
    handle_cmd_helper_send("Resetting local connections...");
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);

    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        ESP32_BT_stop(CONN_SHUTDOWN_DOWNTIME);
    #endif

    setup();
}

// Команда #8
void handle_cmd_set_IoT_flag(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON")) {
        inbuilt_storage_write(cmd_val,
                              strlen(cmd_val),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_FLAG);

        handle_cmd_helper_send("Requests to an IoT server are enabled (ON).");
    } else if (!strcmp(cmd_val, "OFF")) {
        inbuilt_storage_write(cmd_val,
                              strlen(cmd_val),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_FLAG);

        handle_cmd_helper_send("Requests to an IoT server are disabled (OFF).");
    } else {
        handle_cmd_err_val();
    }
}

// Команда #9
void handle_cmd_set_IoT_server_IP(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server target IP changed successfully! New IP is: ",
                          INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                          ECHO_ON);
}

// Команда #10
void handle_cmd_print_IoT_server_IP()
{
    handle_cmd_helper_print("Current IoT server target IP is: ",
                            INBUILT_STORAGE_ADDR_IOT_SERVER_IP);
}

// Команда #11
void handle_cmd_set_IoT_server_port(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server target port changed successfully! New port is: ",
                          INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                          ECHO_ON);
}

// Команда #12
void handle_cmd_print_IoT_server_port()
{
    handle_cmd_helper_print("Current IoT server target port is: ",
                            INBUILT_STORAGE_ADDR_IOT_SERVER_PORT);
}

// Команда #13
void handle_cmd_set_IoT_req_msg(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server request text changed successfully! New text is: ",
                          INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                          ECHO_ON);
}

// Команда #14
void handle_cmd_print_IoT_req_msg()
{
    handle_cmd_helper_print("Current IoT server request text is: ",
                            INBUILT_STORAGE_ADDR_IOT_REQ_MSG);
}

// Команда #15
void handle_cmd_set_IoT_req_period(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server request period changed successfully! New period (ms) is: ",
                          INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
                          ECHO_ON);
}

// Команда #16
void handle_cmd_set_BT_flag(char *cmd)
{
    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        char *cmd_val = strstr(cmd, "=") + 1;

        if (!strcmp(cmd_val, "ON")) {
            inbuilt_storage_write(cmd_val,
                                  strlen(cmd_val),
                                  STR_MAX_LEN,
                                  INBUILT_STORAGE_ADDR_BT_FLAG);

            handle_cmd_helper_send("Bluetooth enabled (ON).");
            handle_cmd_rst_local_conn();
        } else if (!strcmp(cmd_val, "OFF")) {
            inbuilt_storage_write(cmd_val,
                                  strlen(cmd_val),
                                  STR_MAX_LEN,
                                  INBUILT_STORAGE_ADDR_BT_FLAG);

            handle_cmd_helper_send("Bluetooth disabled (OFF).");
            handle_cmd_rst_local_conn();
        } else {
            handle_cmd_err_val();
        }
    #endif
}

// Команда #17
void handle_cmd_set_BT_dev_name(char *cmd)
{
    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        handle_cmd_helper_set(cmd,
                              "Bluetooth device name changed successfully! New name is: ",
                              INBUILT_STORAGE_ADDR_BT_DEV_NAME,
                              ECHO_ON);

        handle_cmd_helper_send("Please reboot your device or reset local connection to put changes into effect.");
    #endif
}

// Команда #18
void handle_cmd_print_BT_dev_name()
{
    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        handle_cmd_helper_print("Current Bluetooth device name is: ",
                                INBUILT_STORAGE_ADDR_BT_DEV_NAME);
    #endif
}

// Команда #19
void handle_cmd_set_RSSI_print_flag(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON")) {
        inbuilt_storage_write(cmd_val,
                              strlen(cmd_val),
                              STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG);

        handle_cmd_helper_send("RSSI print enabled (ON).");
    } else if (!strcmp(cmd_val, "OFF")) {
        inbuilt_storage_write(cmd_val,
                              strlen(cmd_val),
                              STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG);

        handle_cmd_helper_send("RSSI print disabled (OFF).");
    } else {
        handle_cmd_err_val();
    }
}


/*--- Помощники функций-обработчиков ---*/

// Вспомогательная функция для set-обработчиков.
void handle_cmd_helper_set(char *cmd, const char *topic, uint32_t addr, bool echo)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    inbuilt_storage_write(cmd_val,
                          strlen(cmd_val),
                          STR_MAX_LEN,
                          addr);

    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    if (echo) {
        strcat(msg, cmd_val);
    }

    handle_cmd_helper_send(msg);
}

// Вспомогательная функция для print-обработчиков.
void handle_cmd_helper_print(const char *topic, uint32_t addr)
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    char val[INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    inbuilt_storage_read(val,
                         sizeof(val),
                         STR_MAX_LEN,
                         addr);
    strcat(msg, val);
    handle_cmd_helper_send(msg);
}

// Вспомогательная функция для отправки сообщений обработчиками.
void handle_cmd_helper_send(const char *msg)
{
    Serial.println(msg);
    ESP_TCP_server_send_msg(msg);

    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        char val[INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
        inbuilt_storage_read(val,
                             sizeof(val),
                             STR_MAX_LEN,
                             INBUILT_STORAGE_ADDR_BT_FLAG);

        if (!strcmp(val, "ON")) {
            ESP32_BT_send_msg(msg);
        }
    #endif
}


/*--- Прочие функции ---*/

// Чтение настроек со встроенного накопителя устройства в структуру.
void strd_vals_read(strd_vals_t *_strd_vals)
{
    // Индексы в массиве строк, считываемых со встроенного накопителя.
    #define INDEX_SSID               0
    #define INDEX_PSWD               1
    #define INDEX_LOCAL_SERVER_PORT  2
    #define INDEX_IOT_FLAG           3
    #define INDEX_IOT_SERVER_IP      4
    #define INDEX_IOT_SERVER_PORT    5
    #define INDEX_IOT_REQ_MSG        6
    #define INDEX_IOT_REQ_PERIOD     7
    #define INDEX_BT_FLAG            8
    #define INDEX_BT_DEV_NAME        9
    #define INDEX_RSSI_PRINT_FLAG    10


    /*--- Чтение строк со встроенного накопителя ---*/
    
    char strd_vals_str[INBUILT_STORAGE_ITEM_LIST_LEN][INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    uint32_t strd_vals_addr[] = {
        INBUILT_STORAGE_ADDR_SSID,
        INBUILT_STORAGE_ADDR_PSWD,
        INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_FLAG,
        INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
        INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
        INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
        INBUILT_STORAGE_ADDR_BT_FLAG,
        INBUILT_STORAGE_ADDR_BT_DEV_NAME,
        INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG
    };

    uint32_t strd_vals_addr_list_len = sizeof(strd_vals_addr) / sizeof(uint32_t);
    if (INBUILT_STORAGE_ITEM_LIST_LEN != strd_vals_addr_list_len) {
        Serial.println("");
        Serial.println("Error reading from inbuilt storage: function strd_vals_read().");

        return;
    }

    for (uint32_t i = 0; i < INBUILT_STORAGE_ITEM_LIST_LEN; ++i) {
        inbuilt_storage_read(strd_vals_str[i],
                             sizeof(strd_vals_str[i]),
                             INBUILT_STORAGE_STR_MAX_LEN,
                             strd_vals_addr[i]);
    }


    /*--- Запись строк и извлечённых целочисленных значений в структуру ---*/
    
    // SSID.
    strcpy(_strd_vals->SSID, strd_vals_str[INDEX_SSID]);
    
    // Пароль.
    strcpy(_strd_vals->pswd, strd_vals_str[INDEX_PSWD]);
    
    // Порт локального TCP-сервера.
    _strd_vals->local_server_port = strtol(strd_vals_str[INDEX_LOCAL_SERVER_PORT], NULL, 10);

    // Флаг связи с удалённым сервером (сервером IoT).
    if (!strcmp(strd_vals_str[INDEX_IOT_FLAG], "ON")) {
        _strd_vals->IoT_flag = 1;
    } else {
        _strd_vals->IoT_flag = 0;
    }

    // IP удалённого сервера.
    strcpy(_strd_vals->IoT_server_IP, strd_vals_str[INDEX_IOT_SERVER_IP]);
    
    // Порт удалённого сервера.
    _strd_vals->IoT_server_port = strtol(strd_vals_str[INDEX_IOT_SERVER_PORT], NULL, 10);
    
    // Текст запроса, направляемого удалённому серверу.
    strcpy(_strd_vals->IoT_req_msg, strd_vals_str[INDEX_IOT_REQ_MSG]);
    
    // Периодичность отправки запросов удалённому серверу.
    _strd_vals->IoT_req_period = strtol(strd_vals_str[INDEX_IOT_REQ_PERIOD], NULL, 10);

    #if defined THIS_IS_ESP32 && defined BLUETOOTH_PROVIDED
        // Флаг Bluetooth.
        if (!strcmp(strd_vals_str[INDEX_BT_FLAG], "ON")) {
            _strd_vals->BT_flag = 1;
        } else {
            _strd_vals->BT_flag = 0;
        }

        // Имя устройства как ведомого устройства Bluetooth.
        strcpy(_strd_vals->BT_dev_name, strd_vals_str[INDEX_BT_DEV_NAME]);
    #endif

    // Флаг вывода текущего значения RSSI.
    if (!strcmp(strd_vals_str[INDEX_RSSI_PRINT_FLAG], "ON")) {
        _strd_vals->RSSI_print_flag = 1;
    } else {
        _strd_vals->RSSI_print_flag = 0;
    }
}
