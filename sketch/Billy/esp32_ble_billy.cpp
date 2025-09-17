/*************** FILE DESCRIPTION ***************/

/**
 * Filename: esp32_ble_billy.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Bluetooth Low Energy (BLE) UART wrapper functions for ESP32 SoC.
 *           Intended for use with the ESP32 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    Uses the ESP32 BLE Arduino library to emulate a UART service.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "esp32_ble_billy.h"

// Project configs.
#include "config_general.h"

// Arduino core header file.
#include <Arduino.h>

// Conditional compilation.
#if defined ESP32 && defined BLE_USED

// Additional Arduino libraries.
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>


/*************** GLOBAL VARIABLES ***************/

// BLE server and connection state.
BLEServer *BTLE_Server = nullptr;
BLECharacteristic *BTLE_RxCharacteristic = nullptr;
BLECharacteristic *BTLE_TxCharacteristic = nullptr;
bool BTLE_device_connected = false;
std::string BTLE_rx_buffer;


/******************* CALLBACKS ******************/

class BTLE_ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        BTLE_device_connected = true;
    }
    void onDisconnect(BLEServer* pServer) override {
        BTLE_device_connected = false;
    }
};

class BTLE_RxCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) override {
        BTLE_rx_buffer += pCharacteristic->getValue().c_str(); // convert to const char*
    }
};


/******************* FUNCTIONS ******************/

void esp32_ble_billy::start(const char *dev_name)
{
    BLEDevice::init(dev_name);

    BTLE_Server = BLEDevice::createServer();
    BTLE_Server->setCallbacks(new BTLE_ServerCallbacks());

    BLEService *pService = BTLE_Server->createService(BLEUUID((uint16_t)0xFFE0));

    BTLE_TxCharacteristic = pService->createCharacteristic(
        BLEUUID((uint16_t)0xFFE1),
        BLECharacteristic::PROPERTY_NOTIFY
    );
    BTLE_TxCharacteristic->addDescriptor(new BLE2902());

    BTLE_RxCharacteristic = pService->createCharacteristic(
        BLEUUID((uint16_t)0xFFE2),
        BLECharacteristic::PROPERTY_WRITE
    );
    BTLE_RxCharacteristic->setCallbacks(new BTLE_RxCallback());

    pService->start();
    BTLE_Server->getAdvertising()->start();
}

bool esp32_ble_billy::check_connection()
{
    return BTLE_device_connected;
}

uint32_t esp32_ble_billy::read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout)
{
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    uint32_t i = 0, j = 0;
    bool lf = 0;

    while (buf[0] == '\0' && current_millis - previous_millis < conn_timeout && !lf) {
        while (!BTLE_rx_buffer.empty() && current_millis - previous_millis < conn_timeout && !lf) {
            char c = BTLE_rx_buffer.front();
            BTLE_rx_buffer.erase(0, 1);
            ++i;

            if (j < str_max_len) {
                buf[j] = c;
                ++j;
            }

            if (c == '\n') {
                lf = 1;
            }

            current_millis = millis();
            delay(BTLE_READ_SLOWDOWN);
        }
        current_millis = millis();
    }

    return i;
}

void esp32_ble_billy::send_msg(const char *msg)
{
    if (BTLE_device_connected && BTLE_TxCharacteristic) {
        BTLE_TxCharacteristic->setValue((uint8_t*)msg, strlen(msg));
        BTLE_TxCharacteristic->notify();
    }
}

void esp32_ble_billy::disconnect(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    if (BTLE_Server) {
        BTLE_Server->disconnect(0); // Disconnect first connected client
    }
}

void esp32_ble_billy::stop(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    BLEDevice::deinit(true);
}


#endif  // Conditional compilation.
