#pip install paho-mqtt

#python mqtt_producer.py move:0=0:1=0:3=0:5=0:7=0:9=0:2=0:4=0:6=0:8=0
#python mqtt_producer.py move:0=90:1=90:3=90:5=90:7=90:9=90:2=90:4=90:6=90:8=90
#python mqtt_producer.py move:0=-90:1=-90:3=-90:5=-90:7=-90:9=-90:2=-90:4=-90:6=-90:8=-90

import paho.mqtt.client as mqtt
import sys

MQTT_BROKER = "test.mosquitto.org"
MQTT_SEND = "bb7-2.0/servo-driver/in"

client = mqtt.Client()

client.connect(MQTT_BROKER, 1883)
try:
	client.publish(MQTT_SEND, sys.argv[1])
	print("message sent")
except:
	client.disconnect()
	
print("\nNow you can restart fresh")