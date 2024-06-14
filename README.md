![ESP modules](https://i.imgur.com/5ZhI7Su.png)

# Project background

### Concept
Billy is a program (an Arduino sketch) written for ESP32 and ESP8266 microcontrollers.
With this software an MCU can control a load using a digital (ON/OFF) and a pseudo-analog (PWM) output
and receive commands over a UART and wireless networks.

In the following the name "Billy" may refer both to the software and a device that runs it.

### Project pages
**GitHub**
[![github-en](https://img.shields.io/badge/github-en-blue)](https://github.com/ErlingSigurdson/Billy/blob/main/README.md)
[![github-ru](https://img.shields.io/badge/github-ru-blue)](https://github.com/ErlingSigurdson/Billy/blob/main/README.ru.md)

**GitFlic**
[![gitlfic-en](https://img.shields.io/badge/gitflic-en-red)](https://gitflic.ru/project/efimov-d-v/billy/blob?file=README.md&branch=main)
[![gitflic-ru](https://img.shields.io/badge/gitflic-ru-red)](https://gitflic.ru/project/efimov-d-v/billy/blob?file=README.ru.md&branch=main)

### News and milestones
- **14.01.2024** - the sketch is successfully run on a ESP32-C6 module using
[3.0.0-alpha3 version of the ESP32 Arduino core by Espressif Systems](https://github.com/espressif/arduino-esp32/milestone/4).
- **14.06.2024** - v.1.3 released, with certain major changes made:
    - PWM output support added, and thus the project's name changed from "Billy the Relay" to just "Billy".
    - The code was refactored significantly. Among other things, command handler functions
      are now declared and defined in separate files, not in the .ino file.

### TODO list
1. Add a PWM output control panel to the web interface.
2. Add a Bluetooth Low Energy (BLE) support.
3. Add support for switching between station (STA) and access point (AP) Wi-Fi modes,
and afterwards make this switching runtime.


***
# Manual

### Load control
Billy uses 2 GPIO pins to control a load - one for a digital output and another one for a PWM output.

A typical load for Billy would be:
- For a digital output - an electromechanical relay controlled via an octocoupler or a transistor driver.
- For a PWM output - an electric motor controlled via a MOSFET driver.
- For both output types - an LED (usually used for testing purposes).

### Indicator LED
Billy uses a single additional GPIO pin for a digital control over an indicator LED.
The LED indicates a Wi-Fi connection status: a low-frequency blinking for an ongoing connection attempt,
a high-frequency blinking for a successful connection attempts.  

### Combined outputs
A combination of digital and PWM outputs in one pin is to be avoided.
A combination of digital outputs for a load control and for an indicator LED control is safe,
but will effectively turn that same LED into the load to be controlled.

### Communications
Billing can receive commands:
- Over UART through a cable connection.
- Over Wi-Fi as a local TCP server, e.g. sent by a classic Unix utility `netcat` (`nc`)
or by an Anroid app like [Serial Wi-Fi terminal](https://serial-wifi-terminal.en.softonic.com/android).
- Over Wi-Fi as a local HTTP server, e.g. from a web browser (sketch provides a simplistic web interface)
or a different app capable of sending HTTP requests.
- Over Bluetooth Classic as a slave device[^1], e.g. from a desktop Bluetooth terminal or an Android app like
[Serial Bluetooth Controller](https://bluetooth-serial-controller.en.softonic.com/android).
- Over Wi-Fi as a TCP client (inter alia via Internet). To do so Billy sends requests to a custom-programmed TCP server
and receives commands as a response (this implementation is described in detail below).

Billy works within a local Wi-Fi network in a station (STA) mode.
Billy uses an Internet access provided by a local access point.

### IoT: Billy as a TCP client and control over Internet
A remote TCP server (an IoT server) to which Billy sends requests must be able to:
- Send messages (strings) with valid commands in a response to Billy's requests (e.g. string `"UPD_REQ"`).
- Update a message prepared to be sent to Billy according to remote commands sent by other devices.

Say, the IoT server receives "turn load ON" command from your web browser and prepares to send `AT+DLOAD=ON` string
in a response to Billy's next request. Billy receives the string and puts it into a buffer to check for valid commands,
as it would do with a message received over any other communication channel.

[Here you can find an example code for a Linux server that works in a described fashion](https://github.com/ErlingSigurdson/server0451/tree/main)
written in C language. It's been written with an interaction with Billy and similar devices in mind.

### Storing configs
Billy's operations rely heavily on a flash memory storage built into the MCU or into the integrated module.
When you specify configs (an SSID, a password, a port number, etc.), they become saved in the storage
and thus you don't need to assign them again in case of a device reboot.

A current load state (a current digital output level or a PWM duty cycle) isn't considered to be a config though,
and thus it's not stored in the flash storage. Any load will be turned off in case of a device reboot.

### Complete command list
Refer to the file `config_сmd.h` to see all available commands.

### Quickstart
Code configuration and upload, minimal runtime configuration and initial testing are made as follows:

1. In the file `config_general.h`:
- Specify a digital output pin using directive `#define DIGITAL_OUTPUT_PIN`.
- Specify a PWM ouput pin using directive `#define PWM_OUTPUT_PIN`.
- Specify an indicator LED control pin using directive `#define WIFI_INDICATOR_LED_PIN`.
- If your digitally controlled load is turned on by a low logical level on the respective pin,
uncomment the directive `#define INVERTED_DIGITAL_OUTPUT`.
- Comment out the directive `#define BT_CLASSIC_PROVIDED` if your ESP32 device doesn't support Bluetooth Classic
or you just don't want to use this technology. Ignore for ESP8266.
2. Make sure your Arduino IDE (or Arduino SDK for a third-party IDE) has an appropriate core for 
[ESP32](https://github.com/espressif/arduino-esp32) or [ESP8266](https://github.com/esp8266/Arduino)
by Espressif Systems.
3. Compile the sketch and upload it to your device.[^2]
4. Turn on your device and connect to it by a cable (via USB-UART adapter or, if supported by your device,
via UART over native USB).
5. Open a serial terminal and set an appropriate baud rate (115200 by default). 
6. Send command `AT+LOCALSSID=<value>` to specify your Wi-Fi network SSID.
7. Send command `AT+LOCALPSWD=<value>` to specify your Wi-Fi network access point password.
8. Send command `AT+LOCALPORT=<value>` to specify a port number to be used by Billy as a local TCP server.
9. Reset Billy and send a `AT+LOCALCONNRST` command.
10. Make sure your device has established a connection to your Wi-Fi network.
11. Send commands `AT+DLOAD=ON`, `AT+DLOAD=OFF`, `AT+DLOAD=TOGGLE` и `AT+ALOAD=<значение от 0 до 255>`,
over the UART and via a Wi-Fi serial terminal, try controlling your load in different ways.
12. Start any web browser and insert Billy's local IP. Try controlling the load using the web interface.

Test Bluetooth Classic functionality (if supported) as follows:

1. Send command `AT+BTCLASSICDEVNAME=<value>` to specify Billy's name as a slave device.
2. Send command `AT+BTCLASSIC=ON` to turn Bluetooth Classic functionality on.
3. Send commands `AT+DLOAD=ON`, `AT+DLOAD=OFF`, `AT+DLOAD=TOGGLE` and `AT+ALOAD=<value from 0 to 255>`,
over a Bluetooth terminal, try controlling your load in different ways.

Test IoT functionality as follows:

1. Send command `AT+IOTIP=<value>` assign an IP address of a remote server that will send commands to Billy.
2. Send command `AT+IOTPORT=<value>` assign a port number of a remote server opened for the requests from Billy. 
3. Send command `AT+IOTREQMSG=<value>` assign a request message that suits the remote server's settings.
4. Send command `AT+IOT=OFF` turn IoT functionality on.
5. Make sure Billy receives commands from the remote server.


***
# General notes on code

### Sketch layout
Breaking a sketch into a basic `.ino` file and local modules (pairs of `.h` and `.cpp` files) may not be very popular within Arduino paradigm, but I find it more straightforward and easier to manage than simple concatenation of multiple `.ino` files.  

### Global variables
Introduction of additional global variables is generally avoided since the inbuilt storage serves as a "common space" for various sketch functions. Exceptions are made for certain flags and class instances (e.g. `WiFiServer`, `BluetoothSerial`, etc).

### Local modules and wrapper functions
Method calls are mostly packed into wrapper functions declared and defined in local modules. It's handy since method calls are usually accompanied with related lines of code.
Local modules do not refer to each other. Instead, their wrapper functions are combined in `.ino` file, which allows for easier sketch customization.


***
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
