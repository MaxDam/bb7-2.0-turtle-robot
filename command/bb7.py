#pip install paho-mqtt

import paho.mqtt.client as mqtt
import cv2 as cv
import sys
import os
from threading import Thread

#INPUT_RTSP 		= "rtsp://freja.hiof.no:1935/rtplive/_definst_/hessdalen03.stream";
#MQTT_BROKER 		= "192.168.1.8"
INPUT_RTSP 			= "rtsp://192.168.1.6";
MQTT_BROKER 		= "test.mosquitto.org"
MQTT_TOPIC_SERVO 	= "bb7-2.0/servo-driver/in"
MQTT_TOPIC_SERVO_FB = "bb7-2.0/servo-driver/out"
MQTT_TOPIC_DISTANCE = "bb7-2.0/ultrasound_sensor-driver/out"
MQTT_TOPIC_GYRO 	= "bb7-2.0/gyro-driver/out"
MQTT_TOPIC_COMPASS	= "bb7-2.0/compass-driver/out"

HEAD 			 = 0
NECK 			 = 1
RIGHT_FRONT_SHOULDER 	 = 2
RIGHT_FRONT_ARM 	 = 3
LEFT_FRONT_SHOULDER  	 = 4
LEFT_FRONT_ARM 		 = 5
RIGHT_BACK_SHOULDER  	 = 6
RIGHT_BACK_ARM 		 = 7
LEFT_BACK_SHOULDER 	 = 8
LEFT_BACK_ARM 		 = 9

class BB7:
	def __init__(self, acquire_video=True, video_callback=None, video_delay=100, distance_callback=None, gyro_callback=None, compass_callback=None):

		#init telemetry mqtt
		
		self.distance_callback = distance_callback
		self.last_distance = None
		
		self.gyro_callback = gyro_callback
		self.last_gyro = None
		
		self.compass_callback = compass_callback
		self.last_compass = None
		
		self.servo_command = ''
		
		self.connected = False
		
		#init video streaming
		
		self.videoCapture = None
		self.video_callback = video_callback
		self.last_frame = None
		self.video_delay = video_delay

		if acquire_video:
			os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;udp"
			self.videoCapture = cv.VideoCapture(INPUT_RTSP, cv.CAP_FFMPEG)
			self.videoCapture.set(cv.CAP_PROP_BUFFERSIZE, 2)

			# Start frame retrieval thread
			self.video_thread = Thread(target=self._stream_run, args=())
			self.video_thread.daemon = True
			self.video_thread.start()

		#init mqtt
		
		self.mqtt_client = mqtt.Client()
		self.mqtt_client.on_connect = self._on_connect
		self.mqtt_client.on_message = self._on_message
		self.mqtt_client.connect(MQTT_BROKER, 1883)
		self.mqtt_client.loop_start()

	def destroy(self):
		self.mqtt_client.loop_stop()
		if self.videoCapture:
			self.videoCapture.release()
			cv.destroyAllWindows()

	#telemetry mqtt
		
	def _on_connect(self, client, userdata, flags, rc):
		if rc == 0:
			print("Connected to broker")
			self.mqtt_client.subscribe(MQTT_TOPIC_DISTANCE)
			self.mqtt_client.subscribe(MQTT_TOPIC_GYRO)
			self.mqtt_client.subscribe(MQTT_TOPIC_COMPASS)
			self.mqtt_client.subscribe(MQTT_TOPIC_SERVO)
			self.connected = True
		else:
			print("Connection failed")
		
	def _on_message(self, client, userdata, msg):
		print("Message received-> " + msg.topic + " " + str(msg.payload))

		if msg.topic == MQTT_TOPIC_DISTANCE:
			self.last_distance = str(message.payload)
			if self.distance_callback:
				self.distance_callback(self.last_distance)
			
		if msg.topic == MQTT_TOPIC_GYRO:
			self.last_gyro = str(message.payload)
			if self.gyro_callback:
				self.gyro_callback(self.last_gyro)
			
		if msg.topic == MQTT_TOPIC_COMPASS:
			self.last_compass = str(message.payload)
			if self.compass_callback:
				self.compass_callback(self.last_compass)

	def lastDistance(self):
		return self.last_distance;
		
	def lastGyro(self):
		return self.last_gyro;
		
	def lastCompass(self):
		return self.last_compass;
				
	#VIDEO STREAMING

	def _stream_run(self):
		while True:
			if self.videoCapture.isOpened():
				_, frame = self.videoCapture.read()
				frame = cv.resize(frame, (800, 420), cv.INTER_AREA)
				self.last_frame = frame
				
				if self.video_callback:
					self.video_callback(self.last_frame)
			
			time.sleep(self.video_delay)

	def lastFrame(self):
		return self.last_frame

	#SERVO COMMANDS

	def clearCommand(self):
		self.servo_command = ''
		
	def relax(self):
		self.mqtt_client.publish(MQTT_TOPIC_SERVO, "relax")

	def send(self):
		self.servo_command = 'move{}'.format(self.servo_command)
		print("send to " + MQTT_TOPIC_SERVO + ": " + self.servo_command)
		self.mqtt_client.publish(MQTT_TOPIC_SERVO, self.servo_command)
		self.clearCommand()

	def head(self, degree):
		self.servo_command += ':S{}={}'.format(HEAD, degree)
		return self

	def neck(self, degree):
		self.servo_command += ':S{}={}'.format(NECK, degree)
		return self

	def shoulderFrontRight(self, degree):
		self.servo_command += ':S{}={}'.format(RIGHT_FRONT_SHOULDER, degree)
		return self
	
	def armFrontRight(self, degree):
		self.servo_command += ':S{}={}'.format(RIGHT_FRONT_ARM, degree)
		return self

	def shoulderFrontLeft(self, degree):
		self.servo_command += ':S{}={}'.format(LEFT_FRONT_SHOULDER, degree)
		return self
	
	def armFrontLeft(self, degree):
		self.servo_command += ':S{}={}'.format(LEFT_FRONT_ARM, degree)
		return self

	def shoulderBackRight(self, degree):
		self.servo_command += ':S{}={}'.format(RIGHT_BACK_SHOULDER, degree)
		return self
	
	def armBackRight(self, degree):
		self.servo_command += ':S{}={}'.format(RIGHT_BACK_ARM, degree)
		return self

	def shoulderBackLeft(self, degree):
		self.servo_command += ':S{}={}'.format(LEFT_BACK_SHOULDER, degree)
		return self
	
	def armBackLeft(self, degree):
		self.servo_command += ':S{}={}'.format(LEFT_BACK_ARM, degree)
		return self

	def delay(self, millis):
		self.servo_command += ':D={}'.format(millis)
		return self
				
	def arms(self, degree):
		return self.armFrontRight(degree).armFrontLeft(degree).armBackRight(degree).armBackLeft(degree);

	def zero(self, degree=0):
		return self.head(0).neck(0).shoulderFrontRight(0).shoulderFrontLeft(0).shoulderBackRight(0).shoulderBackLeft(0).arms(degree);
