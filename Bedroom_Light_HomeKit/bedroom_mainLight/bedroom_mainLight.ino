#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
 
// Connect to the WiFi
const char* ssid = "*****";
const char* password = "*****";
const char* mqtt_server = "192.168.1.140";
 
WiFiClient espClient;
PubSubClient client(espClient);

//used pin 3, which is normally for serial because on 0 or 1 it was pulling low and would enter flashing mode
const byte ledPin = 3; // Pin with LED on Adafruit Huzzah
 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '1')
  // ESP8266 Huzzah outputs are "reversed"
  digitalWrite(ledPin, HIGH);
  if (receivedChar == '0')
   digitalWrite(ledPin, LOW);
  }
  Serial.println();
}
 
 
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266MainLightClient1")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("bedroom/light/main");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}
 
void setup()
{
 Serial.begin(115200);
 
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 
 pinMode(ledPin, OUTPUT);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
