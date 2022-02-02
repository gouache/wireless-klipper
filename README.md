https://youtu.be/5qxUraexkFY

# wireless-klipper
These are klipper wireless usb-serial based on arduino WifiTelnetToSerial.

|  Android (Pixel 2 XL) | Wemos D1 Mini (STA) | Wemos D1 Mini (AP) | ESP32 (STA) | ESP32 (AP) |
| :----: | :----: | :----: | :----: | :----: |
| Baudrate<br>(115200) | unstable | stable | unstable | stable | 
| Baudrate<br>(250000) | unstable | unstable | unstable | stable |

|  PC     | Wemos D1 Mini (STA) | Wemos D1 Mini (AP) | ESP32 (STA) | ESP32 (AP) |
| :----: | :----: | :----: | :----: | :----: |
| Baudrate<br>(115200) | unstable | stable | unstable | stable | 
| Baudrate<br>(250000) | unstable | stable | unstable | stable |

|  RPI 4 | Wemos D1 Mini (STA) | Wemos D1 Mini (AP) | ESP32 (STA) | ESP32 (AP) |
| :----: | :----: | :----: | :----: | :----: |
| Baudrate<br>(115200) | unstable | stable | unstable | stable | 
| Baudrate<br>(250000) | unstable | unstable | unstable | stable |

|  RPI 3B+ | Wemos D1 Mini (STA) | Wemos D1 Mini (AP) | ESP32 (STA) | ESP32 (AP) |
| :----: | :----: | :----: | :----: | :----: |
| Baudrate<br>(115200) | unstable | stable | unstable | stable | 
| Baudrate<br>(250000) | unstable | unstable | unstable | unstable |

## ESP8266
https://arduino.esp8266.com/stable/package_esp8266com_index.json

## ESP32
https://dl.espressif.com/dl/package_esp32_index.json

## socat
$ sudo apt install socat
$ socat pty,link=/tmp/virtualcom0,ispeed=b115200,ospeed=b115200,raw,echo=0 tcp:192.168.4.1:8888
