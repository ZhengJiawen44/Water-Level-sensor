 /*VCC-3v3
GND-GND
SCL-D18
SDA-D23
RES-D4
DC-D2
CS-D15
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite sprite = TFT_eSprite(&tft);  // Create a sprite object
int prevBlueHeight = -1;

#define rainAnalog 35
#define rainDigital 34
#define LED_PIN 2
#include "index.h"  //Web page header file

WebServer server(80);

//Enter your SSID and PASSWORD
const char* ssid = "iPhone";
const char* password = "123456789";

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {

  int rainAnalogVal = analogRead(rainAnalog);
  int percentage = 100-((rainAnalogVal * 100) / 4095);
  String adcValue = String(percentage) + "%";
  server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleLight() {
  static bool lightState = false;
  lightState = !lightState;
  digitalWrite(LED_PIN, lightState ? HIGH : LOW); // Toggle LED state
  Serial.print("LED turned ");
  Serial.println(lightState ? "on" : "off");
  server.send(200, "text/plain", lightState ? "1" : "0");
}

//===============================================================
// Setup
//===============================================================

void setup(void){

//tft setup
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);  // Start with a black screen
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // White text on black background
  sprite.createSprite(tft.width(), tft.height());  // Create a sprite of the screen size


  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

//LED setup, water setup
  pinMode(LED_PIN, OUTPUT); // Initialize the LED pin as an output
  digitalWrite(LED_PIN, LOW); // Ensure LED is initially off
  pinMode(rainDigital,INPUT);


//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connect to your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
  server.on("/toggleLight", handleLight); // To toggle the LED
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
    int rainAnalogVal = analogRead(rainAnalog);
    int screenHeight = tft.height();
    int blueHeight = map(rainAnalogVal, 0, 4095, screenHeight, 0);  // Map the analog value to the screen height
    int percentage = 100-((rainAnalogVal * 100) / 4095);  // Convert the analog value to a percentage

    if (blueHeight != prevBlueHeight) {
      // Draw to the sprite instead of directly to the screen
      sprite.fillSprite(TFT_BLACK);  // Fill the sprite with black

      // Fill the bottom portion of the sprite with blue based on the analog value
      sprite.fillRect(0, screenHeight - blueHeight, tft.width(), blueHeight, TFT_BLUE);

      // Display the percentage in the center of the sprite
      String percentageStr = String(percentage) + "%";  // Convert the percentage to a String
      sprite.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color to white with black background
      sprite.drawCentreString(percentageStr.c_str(), tft.width() / 2, screenHeight / 2, 4);  // Display the percentage

      // Push the sprite to the screen
      sprite.pushSprite(0, 0);

      prevBlueHeight = blueHeight;
    }


  server.handleClient();
  delay(1);
}