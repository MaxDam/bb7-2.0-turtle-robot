#pip install paho-mqtt

import base64
import cv2 as cv
import numpy as np
import paho.mqtt.client as mqtt

MQTT_BROKER = "test.mosquitto.org"
MQTT_RECEIVE = "bb7-2.0/servo-driver/out"

def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))
	client.subscribe(MQTT_RECEIVE)
	
def on_message(client, userdata, msg):
	print("Message received-> " + msg.topic + " " + str(msg.payload))
	
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(MQTT_BROKER, 1883)
client.loop_start()

while True:
	if cv.waitKey(1) & 0xFF == ord('q'):
		break
	
client.loop_stop()