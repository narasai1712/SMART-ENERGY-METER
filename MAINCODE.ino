#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(D1,D2);  //RX/TX

const char* ssid = "admin";//Replace with your network SSID
const char* password = "12345678";//Replace with your network password
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "ACS712.h"
ESP8266WebServer server(80);
int voltage,current,pf,frequency,power;
   float f1, f2;

String page = "";
ACS712  ACS(A0, 5.0, 1023, 185);
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows

void setup() {
  Serial.begin(115200);
  //lcd.init(); // initialize the lcd
  lcd.backlight();

  lcd.setCursor(0, 0);            // move cursor the first row
  lcd.println("mA:min");          // print message at the first row
  lcd.setCursor(0, 1);            // move cursor to the second row
  lcd.print("mA:max"); // print message at the second row
  lcd.setCursor(0, 2);            // move cursor to the third row
  lcd.print("FF");          // print message at the third row
  while (!Serial);
  Serial.println(__FILE__);
  Serial.print("ACS712_LIB_VERSION: ");
  Serial.println(ACS712_LIB_VERSION);


  ACS.autoMidPoint();

  //
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.on("/", []()
  {
    page = "<html><head><title>Smart Energy Meter using IoT</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color:  green ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Smart Energy Meter using IoT</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>Parameters</th><th>Value</th><th>Units</th></tr><tr><td>Voltage</td><td>"+String(voltage)+"</td><td>Volts</td></tr>";
    page += "<tr><td>Current</td><td>"+String(f1)+"</td><td>mili Amperes</td></tr><tr><td>Power Factor</td><td>"+String(pf)+"</td><td>--</td>";
    page += "<tr><td>Power</td><td>"+String(f2)+"</td><td>Watts</td></tr><tr>";
    page += "</tr><tr><td>Frequency</td><td>"+String(frequency)+"</td><td>Hz</td></tr>";
    page += "</tr><tr><td>Price Per Unit</td><td>5 INR</td><td>Unit</td></tr>";
    page += "</tr><tr><td>Total Units Consumed</td><td>"+String(f1*5.0)+"</td><td>INR</td></tr>";
    page += "<meta http-equiv=\"refresh\" content=\"3\">";
    server.send(200, "text/html", page);
  });
  server.begin();
}

void loop() {
    f1 = ACS.mA_AC();
   f2 = ACS.mA_AC_sampling();
  voltage = 230;
//current = pzem.current();
//power = pzem.power();
frequency = 50;
pf = pzem.pf();
server.handleClient();

  lcd.setCursor(7, 0);
  lcd.println(f1,1);
  lcd.setCursor(7, 1);
  lcd.println(f2,1);
  
   
 
  Serial.println("mA:min ");
  Serial.println(f1, 1);
  

   Serial.println("mA:max");
   Serial.println(f2, 1);
Serial.println("FF: ");
   Serial.println(f1 / f2);
   lcd.setCursor(4, 2);
  lcd.println(f1/f2,2);




  delay(1000);
}
