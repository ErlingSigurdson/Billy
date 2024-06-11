[<65;80;20M]![ESP modules](https://i.imgur.com/5ZhI7Su.png)

# Project background information

### Concept
Billy is a program (an Arduino sketch) written for ESP32 and ESP8266 microcontrollers.
With this software an MCU can control a digital (ON/OFF) and an analog (with PWM) load
and receive commands over a UART and wireless networks. In the following the name "Billy"
may refer both to the software and a device that runs it.

### Project pages
**GitHub**
[![github-en](https://img.shields.io/badge/github-en-blue)](https://github.com/ErlingSigurdson/Billy/blob/main/README.md)
[![github-ru](https://img.shields.io/badge/github-ru-blue)](https://github.com/ErlingSigurdson/Billy/blob/main/README.ru.md)

**GitFlic**
[![gitlfic-en](https://img.shields.io/badge/gitflic-en-red)](https://gitflic.ru/project/efimov-d-v/billy/blob?file=README.md&branch=main)
[![gitflic-ru](https://img.shields.io/badge/gitflic-ru-red)](https://gitflic.ru/project/efimov-d-v/billy/blob?file=README.ru.md&branch=main)

### News and milestones
- **14.01.2024** - the sketch is successfully run on a ESP32-C6 microcontroller using
[3.0.0-alpha3 version of the ESP32 Arduino core](https://github.com/espressif/arduino-esp32/milestone/4).
- **__.06.2024** - v.1.3 released, with certain major changes made:
    - analog load control functionality added, and thus the project's name changed
      from "Billy the Relay" to just "Billy";
    - the code was refactored significantly. Among other things, command handler functions
      are now declared and defined in separate files, not in the .ino file.

### TODO list
- Add switching between station (STA) and access point (AP) Wi-Fi modes in runtime.
- Add an analog load control panel to the web interface.


***
# Manual

### GPIO and Load
Billy provides 2 control channels - one for a digital load and another one for an analog load.
The channels are basically GPIOs, one in a digital output mode and another one outputting PWM signal.

A typical digital load would be an LED (usually used for testing purposes) or a transistor driver which
in its turn switches a relay. A typical analog load would be, again, an LED or a MOSFET driver which
in its turn drives an electric motor.

Additionally, there's a GPIO in a digital output mode assigned to control an indicator LED.

GPIOs are assigned with a `#define` directive in 


### Communications

### Storing configs


Billy is a sketch for Arduino framework written for ESP32 and ESP8266 modules (systems-on-chip, SoCs).
The same name may also refer to a device that runs the respective sketch. 
Billy can control:
- a digital (ON/OFF) load, like an LED or (by means of a transistor driver) a relay;
- an analog load, like an LED of (by means of a transistor driver) an electric motor.

Load is controlled by changing the output voltage at GPIOs specified in the code.

Billing can receive commands:
- over UART through a cable connection;
- over Wi-Fi as a local TCP server, e.g. sent by a classic Unix utility `netcat` (`nc`)
or by an Anroid app like [Serial Wi-Fi terminal](https://serial-wifi-terminal.en.softonic.com/android);  
- over Wi-Fi as a local HTTP server, e.g. from a web browser (sketch provides a simplistic web interface)
or a different app capable of sending HTTP requests;
- over Bluetooth Classic as a slave device[^1], e.g. from a desktop Bluetooth terminal or an Android app like
[Serial Bluetooth Controller](https://bluetooth-serial-controller.en.softonic.com/android);
- over Wi-Fi as a TCP client (inter alia via Internet). To do so Billy sends requests to a custom-programmed TCP server
and receives commands as a response (this implementation is described in detail below).

Billy works within a local Wi-Fi network in a station (STA) mode.
Billy uses an Internet access provided by a local access point.

Billy's operations rely heavily on a module's inbuilt flash memory storage.
When you specify configs (an SSID, a password, a port number, etc.), they are saved in the inbuilt storage
and thus you don't need to assign them again in case of a device reboot.
There's a single exception: load state is reset to OFF (or 0 duty cycle) upon reset.


# Manual
### Complete command list
Please refer to `config_сmd.h` to see all the available commands.

### Quickstart
Follow these steps to configure and start using Billy:
1. In the file `config_general.h` use `#define` directives to specify:
- whether your device uses Bluetooth Classic (`BT_CLASSIC_PROVIDED`). Comment out the directive if your ESP32
doesn't support Bluetooth Classic or you just don't want to use it. Ignore for ESP8266;
- digital load control pin (`DIGITAL_LOAD_PIN`). Specify the pin number as a value;
- analog load control pin (`ANALOG_LOAD_PIN`). Specify the pin number as a value;
- indicator LED control pin (`WIFI_INDICATOR_LED_PIN`). Specify the pin number as a value;
- digital load mode (`INVERTED_DIGITAL_OUTPUT`). Uncomment the directive if you need an inverted output.
2. Make sure your Arduino IDE (or Arduino SDK for a third-party IDE) has an appropriate core for 
[ESP32](https://github.com/espressif/arduino-esp32) or [ESP8266](https://github.com/esp8266/Arduino)
by Espressif Systems.
3. Compile the sketch and upload it to your device.[^2]
4. Turn on your device and connect to it by a cable (through USB-UART adapter or, if suppored, UART over native USB).
Open serial terminal.
5. Send command `AT+DLOAD=TOGGLE` twice and make sure that Billy switches a current digital load status.
6. Consecutively send commands `AT+ALOAD=255` and `AT+ALOAD=00` and make sure that Billy control the analog load.

7. Send command `AT+LOCALSSID=<value>` to specify your local Wi-Fi network SSID.
8. Send command `AT+LOCALPSWD=<value>` to specify your local Wi-Fi network access point password.
9. Send command `AT+LOCALPORT=<value>` to specify port number to be used by your device as a local TCP server.
10. Reset your device or send the command `AT+RSTLOCALCONN`. Make sure your device has established a connection to your local Wi-Fi network (look at UART terminal and indicator LED). Remember of write down printed local IP address (you can use `AT+PRINTLOCALIP` command to print it again if necessary).
11. Connect to you device over Wi-Fi using previously printed local IP address and previously specified port.
12. Try sending commands (e.g. `AT+SETLOAD=TOGGLE`) over Wi-Fi using established TCP connection. Make sure Billy follows your instructions.
13. Open any web browser (I recommend Mozilla Firefox) on any device connected to the same local Wi-Fi network and insert previously printed IP address into an address bar.
14. Use web interface to turn your load ON and OFF.

Additionally, if your device supports Bluetooth Classic:

15. Send command `AT+SETBTDEVNAME=<value>` to specify Billy's name as a Bluetooth slave device.
16. Send command `AT+SETBT=ON` to turn on Bluetooth Classic.
17. Connect to Billy over Bluetooth using previously specified slave device name.
18. Try sending commands (e.g. `AT+SETLOAD=TOGGLE`) over Bluetooth. Make sure Billy follows your instructions.

### Billy as a TCP client and IoT control
A remote server (an IoT server) to which Billy sends requests must be able to:
- in response to Billy's requests (e.g. string `"UPD_REQ"`) send messages (strings) with valid commands;
- update a message prepared to be sent to Billy according to remote commands sent by other devices.

Say, IoT server receives "turn load ON" command from your web browser and prepares to send `AT+SETLOAD=ON` string in a response to Billy's next request. Billy receives the string and puts it into a buffer to check for valid commands.

To specify IoT configs use commands `AT+SETIOTIP=<value>`, `AT+SETIOTPORT=<value>`, `AT+SETIOTREQMSG=<value>`, `AT+SETIOTREQPERIOD=<value>`, `AT+SETIOT=ON` and `AT+SETIOT=OFF`.

[Here you can find an example code for a Linux server that works in a described fashion](https://github.com/ErlingSigurdson/server0451/tree/main) written in C language. It is written specifically for Billy and similar devices.

### Notes on GPIO
Defining `DIGITAL_LOAD_PIN` and `ANALOG_LOAD_PIN` with same values should be avoided
since it will most probably render digital control inoperable.
Defining `DIGITAL_LOAD_PIN` and `WIFI_INDICATOR_LED_PIN` is technically acceptable, but hardly makes sense
unless your digital load is an LED.


# General notes on code
### Sketch layout
Breaking a sketch into a basic `.ino` file and local modules (pairs of `.h` and `.cpp` files) may not be very popular within Arduino paradigm, but I find it more straightforward and easier to manage than simple concatenation of multiple `.ino` files.  

### Global variables
Introduction of additional global variables is generally avoided since the inbuilt storage serves as a "common space" for various sketch functions. Exceptions are made for certain flags and class instances (e.g. `WiFiServer`, `BluetoothSerial`, etc).

### Local modules and wrapper functions
Method calls are mostly packed into wrapper functions declared and defined in local modules. It's handy since method calls are usually accompanied with related lines of code.
Local modules do not refer to each other. Instead, their wrapper functions are combined in `.ino` file, which allows for easier sketch customization.


# Expected questions
### Why not MQTT? It's so well-suited for IoT!
I wanted more flexibility and I didn't want to stick to a particular OSI layer 7 protocol.

### Why "Billy"?
A reference to an eponymous programmer parrot, a protagonist of [meme videos](https://www.youtube.com/watch?v=0MhVkKHYUAY&list=PLkdGijFCNuVmu35l6EJxdvsvf7xj4EQVf&index=21) by [Mr. P Solver](https://www.youtube.com/c/mrpsolver).

### What about security?
Within your local Wi-Fi network your best protection is your access point password. You can even run additional Wi-Fi network on the same router if you want to separate Billy and other IoT devices from your regular consumer electronics.

As for Bluetooth (and lack of PIN code for BT access), you can change a command syntax in `config_ASCII_cmd_handler.h` and make commands look like `AT+` `MYPSWD_SETLOAD=TOGGLE`.

[^1]: Not supported for ESP8266 and those ESP32 modules which lack Bluetooth Classic functionality (e.g. ESP32-S2 and ESP32-C3).
[^2]: In case of your IDE throwing a message on insufficient memory size try switching `partition scheme` option to "Huge APP".
