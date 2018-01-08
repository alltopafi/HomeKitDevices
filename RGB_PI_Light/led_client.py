#!/usr/bin/env python

import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO

GPIO.setwarnings(False)

print("connecting...")

def on_connect(client, userdata, rc):
    print ("Connected with rc: " + str(rc))
    client.subscribe("rgbLight")

def on_message(client, userdata, msg):
    print ("Topic: "+ msg.topic+"\nMessage: "+str(msg.payload))

    if "hue" in msg.payload:
       return

    if "power" in msg.payload:
        return


    if "saturation 20" in msg.payload:
        print("Green on")
        GPIO.output(11,True)
    else:
        GPIO.output(11,False)
    if "saturation 68" in msg.payload:
       print("blue on")
       GPIO.output(12,True)
    else:
       GPIO.output(12,False)
    if "saturation 67" in msg.payload:
        print("red on")
        GPIO.output(13,True)
    else:
        GPIO.output(13,False)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("10.0.0.11", 1883, 60)

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)
GPIO.setup(12, GPIO.OUT)
GPIO.setup(13, GPIO.OUT)

client.loop_forever()