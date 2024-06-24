# Water-Level-sensor
ESP32 Water Level Sensor and TFT Display Project

Components Used
ESP32 Development Board: Used as the main microcontroller.
TFT Display (ST7735): Display module for visual output.
Water Level Sensor: Analog sensor to measure water level.
LED: Indicator controlled via the web server.
Pin Configuration
TFT Display:

VCC - 3.3V
GND - Ground
SCL - D18
SDA - D23
RES - D4
DC - D2
CS - D15
Other Components:

LED_PIN: GPIO pin for controlling an LED.
rainAnalog: Analog pin connected to the water level sensor.
rainDigital: Digital pin for supplementary control (if used).
Libraries Used
WiFi.h: ESP32 WiFi library for connecting to a network.
WebServer.h: Library to create a web server on the ESP32.
TFT_eSPI.h: Graphics library for the ST7735 TFT display.
