#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Connect to the WiFi
const char* ssid = "*****";
const char* password = "*****";
const char* mqtt_server = "192.168.1.140";
 
WiFiClient espClient;
PubSubClient client(espClient);

// Hardware setup
const int buttonPin = 2;     // the number of the pushbutton pin

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int numberOfClicks = 0;
bool powerOn = false;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Button Client1123")) {
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

// Bring up the serial for a bit of debugging
  Serial.begin(115200);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Set your pin modes
  pinMode(buttonPin, INPUT);
  client.setServer(mqtt_server, 1883);
//  client1.setCallback(callback);
  
  // Bring up the serial for a bit of debugging
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  

  Serial.println("setup is done");

}

void loop() 
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) 
    {
      buttonState = reading;

      // When the button is in the LOW state (pulled high) the button 
      // has been pressed so send the event.
      Serial.print("numberOfClicks = ");
      Serial.println(numberOfClicks);
      if (buttonState == LOW) {
        if(powerOn){
        //turn off
        client.publish("bedroom/light/main","0");
        client.publish("bedroom/bedsideLight","0");
        Serial.println("off");
        
        powerOn=false;
        }else{
          //turn on
          if(numberOfClicks == 0) {
            client.publish("bedroom/light/main","1");
            Serial.println("main light on");
            numberOfClicks++;
            Serial.println(numberOfClicks);
//            powerOn=true;
          }else{
            if(numberOfClicks == 1) {
              client.publish("bedroom/bedsideLight","1");
              Serial.println("bedside light on");
              numberOfClicks=0;
              powerOn=true;
            }
          }
        }
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

//void send_event(const char *event)
//{
//  // set the LED on whle we are sending the event
//  digitalWrite(ledPin, HIGH);
//
//  Serial.print("Connecting to ");
//  Serial.println(host);
//  
//  // Use WiFiClient class to create TCP connections
//  WiFiClient client;
//  const int httpPort = 80;
//  if (!client.connect(host, httpPort)) {
//    Serial.println("Connection failed");
//    return;
//  }
//  
//  // We now create a URI for the request
//  String url = "/trigger/";
//  url += event;
//  url += "/with/key/";
//  url += privateKey;
//  
//  Serial.print("Requesting URL: ");
//  Serial.println(url);
//  
//  // This will send the request to the server
//  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" + 
//               "Connection: close\r\n\r\n");
//
//  // Read all the lines of the reply from server and print them to Serial,
//  // the connection will close when the server has sent all the data.
//  while(client.connected())
//  {
//    if(client.available())
//    {
//      String line = client.readStringUntil('\r');
//      Serial.print(line);
//    } else {
//      // No data yet, wait a bit
//      delay(50);
//    };
//  }
//  
//  // All done
//  Serial.println();
//  Serial.println("closing connection");
//
//  client.stop();
//  
//  // Finished sending the message, turn off the LED
//  digitalWrite(ledPin, LOW);
//}
