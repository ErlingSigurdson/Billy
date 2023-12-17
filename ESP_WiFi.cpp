/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: ESP_WiFi.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: управление встроенным интерфейсом WiFi для модулей
 * ESP32 и ESP8266.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Основная библиотека Arduino.
#include <Arduino.h>

// Локальные модули.
#include "ESP_WiFi.h"

// Дополнительные библиотеки для Arduino IDE.
#ifdef THIS_IS_ESP32
    #include <WiFi.h>         // Вариант для ESP32.
#endif
#ifdef THIS_IS_ESP8266
    #include <ESP8266WiFi.h>  // Вариант для ESP8266.
#endif


/******************** ФУНКЦИИ *******************/

bool ESP_WiFi_set_connection(char *SSID, char *pswd, uint32_t conn_attempt_timeout)
{
    if (WiFi.isConnected()) {
        WiFi.disconnect();
    }
    while (WiFi.isConnected());  // Пауза, гарантирующая отключение предыдущего соединения с сетью Wi-Fi.

    Serial.print("Connecting to Wi-Fi access point ");
    Serial.println(SSID);

    WiFi.begin(SSID, pswd);
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);             // Точка выводится раз в секунду.
        Serial.print(".");

        current_millis = millis();
        if (current_millis - previous_millis >= conn_attempt_timeout) {
            Serial.println("");
            Serial.println("Connection timeout. Please set valid SSID and password and/or restart the connection.");

            return 0;
        }
    }

    Serial.println("");
    Serial.println("Connected to Wi-Fi network!");

    return 1;
}

void ESP_WiFi_indicate_connection(uint32_t led_pin, uint32_t cycles, uint32_t period)
{
    if (cycles % 2) {
        ++cycles;
    }

    for (uint32_t i = 0; i < cycles; ++i) {
        digitalWrite(led_pin, !digitalRead(led_pin));
        delay(period);
    }

    return;
}

// Изначально хотел сделать возвращаемым типом IPAddress, но тогда скетч почему-то не компилировался для ESP8266.
String ESP_WiFi_get_current_IP()
{
    return WiFi.localIP().toString();
}

void ESP_WiFi_RSSI_print()
{
    int64_t RSSI = WiFi.RSSI();
    Serial.print("Current RSSI is: ");
    Serial.println(RSSI);
}
