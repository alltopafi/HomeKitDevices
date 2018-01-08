/*
An open-source LM35DZ Temperature Sensor for Arduino. This project will be enhanced on a regular basis
(cc) by Daniel Spillere Andrade , http://www.danielandrade.net
http://creativecommons.org/license/cc-gpl
*/

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "*****";
const char* password = "*****";

int pin = 0; // analog pin
int tempc = 0,tempf=0; // temperature variables
int samples[60]; // variables to make a better precision
int maxi = -100,mini = 100; // to start max/min temperature
int i;
void setup()
{
  Serial.begin(9600); // start serial communication

  WiFi.begin(ssid, password);

  Serial.print("Waiting for connection");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected!");

}

void loop()
{
  
  
for(i = 0;i<=59;i++){ // gets 8 samples of temperature
  
  samples[i] = ( 5.0 * analogRead(pin) * 100.0) / 1024.0;
  tempc = tempc + samples[i];
  delay(1000);

}

tempc = tempc/59.0; // better precision
tempf = (tempc * 9)/ 5 + 32; // converts to fahrenheit

if(tempc > maxi) {maxi = tempc;} // set max temperature
if(tempc < mini) {mini = tempc;} // set min temperature

Serial.print(tempc,DEC);
Serial.print(" Celsius, ");

Serial.print(tempf,DEC);
Serial.print(" fahrenheit -> ");

Serial.print(maxi,DEC);
Serial.print(" Max, ");
Serial.print(mini,DEC);
Serial.println(" Min");

tempc = 0;

sendTempToServer(tempf);  

}


void sendTempToServer(int temp){
  if(WiFi.status() == WL_CONNECTED) {

  HTTPClient http;

  http.begin("http://192.168.1.140/temprequest.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String data = "temp="+String(temp);
  int httpCode = http.POST(data);
  String payload = http.getString();

  Serial.println();
  Serial.println(httpCode);
  Serial.println(data);
  Serial.println(payload);
  Serial.println();

  http.end();

  }else{
    Serial.println("Error in Wifi connection");
  }
  delay(1000); // delay before loop
}

