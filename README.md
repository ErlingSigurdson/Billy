# Concept
Billy the Relay is an Arduino project written for ESP32 and ESP8266 modules (systems-on-chip). Billy can control simple (ON/OFF) load like an LED or (by means of a driver) a relay.
Billy itself takes commands via:
- UART, through a cable connection.
- Wi-Fi as a local TCP server, e.g. from a classic Unix utility `netcat` (`nc`) or an Anroid app like [Serial Wi-Fi terminal](https://serial-wifi-terminal.en.softonic.com/android).   
- Wi-Fi as a local HTTP server, e.g. from a browser (sketch provides a simplistic web interface) or a different app capable of sending HTTP requests.
- Bluetooth Classic as a slave device[^1], e.g. from a desktop Bluetooth terminal or an Android app like [Serial Bluetooth Controller](https://bluetooth-serial-controller.en.softonic.com/android).
- Wi-Fi as a TCP client (inter alia via Internet). To do so Billy sends requests to a custom-programmed TCP server and receives commands as a response (this implementation is described in detail below).

Billy works within a local Wi-Fi network in a station (STA) mode. Billy uses Internet access provided by a local access point.

# Manual
### Quickstart
Follow these steps to configure Billy and start using it:
1. In file `config_general.h` set `#define` values to specify:
- whether your device uses Bluetooth Classic (`BT_CLASSIC_PROVIDED`, for ESP32 only; omit for ESP8266);
- load control pin (`LOAD_PIN`);
- indicator LED control pin (`WIFI_INDICATOR_LED_PIN`);
- load mode (`INVERTED_OUTPUT`).
2. Make sure your Arduino IDE (or Arduino SDK for a third-party IDE) has an appropriate core for [ESP32](https://github.com/espressif/arduino-esp32) or [ESP8266](https://github.com/esp8266/Arduino) by Espressif Systems.
3. Compile sketch and upload the sketch.[^2]
4. Turn on your device and connect to it by a cable (through USB-UART adapter or, if suppored, UART over native USB).
5. Send command `AT+SETLOAD=TOGGLE` twice and make sure that load control pin's logical level changes every time (load turns ON and OFF).
6. Send command `AT+SETLOCALSSID=<value>` to specify your local Wi-Fi network SSID.
7. Send command `AT+SETLOCALPSWD=<value>` to specify your local Wi-Fi network access password.
8. Send command `AT+SETLOCALPORT=<value>` to specify port number to be used by your device as a local TCP server.
9. Reset your device or send command `AT+RSTLOCALCONN`. Make sure you device has established a connection to your local Wi-Fi network (watch UART terminal and indicator LED). Remember of write down printed local IP address (you can use `AT+PRINTLOCALIP` command to print it again).
10. Connect to you device by Wi-Fi using previously printed local IP address and previously specified port.
11. Try sending commands (e.g. `AT+SETLOAD=TOGGLE`) over Wi-Fi using established TCP connection. Make sure Billy follows your orders.
12. Open any web browser (I recommend Mozilla Firefox) on any device connected to the same local Wi-Fi network and insert previously printed IP address into an address bar.
13. Use web interface to turn your load ON and OFF.

Additionally, if your device supports Bluetooth Classic:

14. Send command `AT+SETBTDEVNAME=<value>` to specify Billy's name as a Bluetooth slave device.
15. Send command `AT+SETBT=ON` to turn on Bluetooth Clasicc.
16. Connect to Billy by Bluetooth using previously specified slave device name.
17. Try sending commands (e.g. `AT+SETLOAD=TOGGLE`) over Bluetooth. Make sure Billy follows your orders.

### Complete command list
Please refer to `config_ASCII_cmd_handler.h`.

### Billy as a TCP client and IoT control
A remote server (an IoT server) to which Billy sends requests must be able to:
- in response to Billy's requests (e.g. string `"UPD_REQ"`) send messages (strings) with valid commands;
- update a message to be sent to Billy according to remote commands sent by another devices.

Say, IoT server receives "turn load ON" command from your web browser and prepares to send `AT+SETLOAD=ON` string in a response to Billy's next request. Billy receives the string and puts it into a buffer to check for valid commands.

To specify IoT configs use commands `AT+SETIOTIP=<value>`, `AT+SETIOTPORT=<value>`, `AT+SETIOTREQMSG=<value>`, `AT+SETIOTREQPERIOD=<value>`, `AT+SETIOT=ON` and `AT+SETIOT=OFF`.

[Here you can find an example code for a Linux server that works in a described fashion, written in C language.](https://github.com/ErlingSigurdson/server0451/tree/main). It is written specifically for Billy and similar devices.

# General notes on code
### Sketch layout
Breaking a sketch into a basic `.ino` file and local modules (pairs of `.h` and `.cpp` files) may not be very popular within Arduino paradigm, but I find it more straightforward and easier to manage than simple concatenation of multiple `.ino` files.  

### Global variables
Use of global variables is generally avoided since the inbuilt storage serves as a "common space" for various sketch functions. Exceptions made for certain flags and class instances (e.g. `WiFiServer`, `BluetoothSerial`, etc).

### Local modules and wrapper functions
Method calls are mostly packed into wrapper functions declared in local modules. It's handy since method calls are usually accompanied with related lines of code.
Local modules do not refer to each other. Instead, their wrapper functions are combined in `.ino` file, which allows for easier sketch customization.

# Expected questions
### Why not MQTT? It's so well-suited for IoT!
I wanted more flexibility and I didn't want to stick to a particular OSI layer 7 protocol.

### Why "Billy"?
An honorable mention of a programmer parrot, protagonist of [meme videos](https://www.youtube.com/watch?v=0MhVkKHYUAY&list=PLkdGijFCNuVmu35l6EJxdvsvf7xj4EQVf&index=21) by [Mr. P Solver](https://www.youtube.com/c/mrpsolver).

[^1]: Not supported for ESP8266 and those ESP32 modules which lack Bluetooth Classic functionality (e.g. ESP32-S2 and ESP32-C3).
[^2]: If your IDE warns you of lack of memory for storing the sketch, try switching `partition scheme` option to "Huge APP".
