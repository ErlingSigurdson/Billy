# Концепция
Билли - это Arduino-проект на базе модуля (платы) ESP32 или ESP8266. Билли умеет управлять простой (ВКЛ./ВЫКЛ.) нагрузкой, например, светодиодом или (с помощью драйвера) реле.
В свою очередь Билли можно отдавать команды:
+ По UART через провод.
+ По Wi-Fi как локальному TCP-серверу, например, с помощью классической Unix-утилиты `netcat` или приложения для Android [Serial Wi-Fi terminal](https://serial-wifi-terminal.en.softonic.com/android).   
+ По Bluetooth Classic как ведомому устройству, например с помощью приложения для Android [Serial Bluetooth Controller](https://bluetooth-serial-controller.en.softonic.com/android).
+ По Wi-Fi (в том числе через Интернет) как TCP-клиенту. Для этого Билли направляет запросы специально запрограммированному серверу и получающает от него команды. 

## Прочее
### Почему не MQTT?
Хотелось обеспечить больше гибкости.
### Почему "Билли"?
В честь попугая-программиста, героя [мемных видео](https://www.youtube.com/watch?v=0MhVkKHYUAY&list=PLkdGijFCNuVmu35l6EJxdvsvf7xj4EQVf&index=21) от [Mr. P Solver](https://www.youtube.com/c/mrpsolver).

