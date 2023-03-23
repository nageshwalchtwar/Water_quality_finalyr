#include <SPI.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <EEPROM.h>
#include <GravityTDS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
#define TdsSensorPin 35
GravityTDS gravityTds;
const int oneWireBus = 27;     
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
#define i2c_Address 0x3c 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const char* ssid     = "SCRC_LAB_IOT";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Scrciiith@123"; 
const char* server = "http://api.thingspeak.com";
// Service API Key
unsigned long Channel_ID = 1816757;  //replace with your Channel ID
const char * API_key = "8JQ08D54RO47H2NC";

WiFiClient client;

unsigned long last_time = 0;
unsigned long Delay = 30000;
//end for thingspeak script

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define OLED_SDA 21
#define OLED_SCL 22

#define relay1 16
#define relay2 17


void setup()   {

  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
 
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
      Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());// Send the IP address of the ESP8266 to the computer
  Serial.println(WiFi.macAddress());
  }
  delay(250); 
  display.begin(i2c_Address, true);
  sensors.begin(); 
 //display.setContrast (0); // dim display
  pinMode(OLED_SDA, INPUT_PULLUP);     
  pinMode(OLED_SCL, INPUT_PULLUP); 
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(35,INPUT);

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(3.3);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 20);
  display.print("Water Quality");
  display.setCursor(40, 40);
  display.print("Test");
  display.display();
  delay(2000);
  display.clearDisplay();
  //thingspeak script starts here
  // WiFi.mode(WIFI_STA);
  
  // ThingSpeak.begin(client);


}


void loop() {
    digitalWrite(relay2,HIGH);
    delay(7000);
    digitalWrite(relay2,LOW);
    delay(1000);
    //gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    digitalWrite(relay1,HIGH);
    delay(4500);
    gravityTds.update();  //sample and calculate
    float tdsValue = gravityTds.getTdsValue();  // then get the value
 
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    
    digitalWrite(relay1,LOW);
    delay(1000);
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    Serial.print(temperatureC);
    Serial.println("ºC");
    digitalWrite(relay2,HIGH);
    delay(15000);
    digitalWrite(relay2,HIGH);
    display.clearDisplay();
//tds 
   display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.println("TDS:");
     display.display();
    display.setCursor(0,30);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.println(tdsValue,0);
 display.display();
    display.setCursor(0,50);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.println(" PPM ");
    display.display();

    // Temperarture
    display.setCursor(64,0);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.println("Temp:");
 display.display();
    display.setCursor(64,30);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.println(temperatureC);
 display.display();
    display.setCursor(64,50);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.println(" �C");
    display.display();
    delay(10000);


}