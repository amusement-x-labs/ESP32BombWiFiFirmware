# ESP32BombWiFiFirmware
Simple ESP32 firmware to control virtual BOMB via WiFi

Make sure that [arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets) library is setup in Arduino IDE Library manager.

Base connection scheme:
![scheme](https://github.com/amusement-x-labs/ESP32BombWiFiFirmware/blob/main/Scheme.png)

Pins connection:
| ESP32 PIN | SCHEME |
| ------ | ------ |
| D21 | Resistor |
| D22 | Resistor |
| D23 | Resistor |
| GND | LEDs' cathodes (small one) |

## Related repos:
* Oculus quest VR / Unity: https://github.com/amusement-x-labs/OculusQuestBombESP32WiFi
* Web Server: https://github.com/amusement-x-labs/WebServerESP32BombControl

## Fullfit demonstration
https://youtu.be/bGzesUlRl3g