//Preparazione IDE ESP32:
//File > Preferences > Additional Board Manager URLs > https://dl.espressif.com/dl/package_esp32_index.json
//Tools > Board Arduino Uno > Boards Manager > ESP32
//Tools > Board Arduino Uno > ESP32 Arduino" > DOIT ESP32 DEVKIT V1
//Tools > Manage Libraries > PubSubClient
//Tools > Manage Libraries > ArduinoJson
//Tools > Manage Libraries > Adafruit PWM Servo Driver Library by Adafruit
//Tools > Port > <Selezionare porta seriale disponibile>

//links:
//https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
//https://techtutorialsx.com/2017/04/24/esp32-connecting-to-a-wifi-network/
//https://learn.adafruit.com/16-channel-pwm-servo-driver/using-the-adafruit-library
//https://robojax.com/learn/arduino/?vid=robojax_PCA9685-V4
//https://www.pubnub.com/blog/pubsub-nodemcu-32s-esp32-mqtt-pubnub-arduino-sdk/
//https://arduinojson.org/v6/example/string/
//https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <ArduinoJson.h>

//servo range
#define DEGREEMIN -90 //'minimum' degree
#define DEGREEMAX 90  //'maximum' degree
#define SERVOMIN  150 // 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // 'maximum' pulse length count (out of 4096)

//body joint
#define HEAD 				 0
#define NECK 				 1
#define RIGHT_FRONT_SHOULDER 2
#define RIGHT_FRONT_ARM		 3
#define LEFT_FRONT_SHOULDER	 4
#define LEFT_FRONT_ARM	     5
#define RIGHT_BACK_SHOULDER	 6
#define RIGHT_BACK_ARM 		 7
#define LEFT_BACK_SHOULDER 	 8
#define LEFT_BACK_ARM	     9

//wifi and mqtt configuration
const char* ssid     = "Vodafone-C01960075";
const char* password = "tgYsZkgHA4xhJLGy";
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* topic_input = "bb7-2.0/servo-driver/in";
const char* topic_output = "bb7-2.0/servo-driver/out";
const char* client_id = "bb7-2.0/servo-driver/123456";

//mqtt vars
WiFiClient MQTTclient;
PubSubClient client(MQTTclient);
long lastReconnectAttempt = 0;

//adafruit servo motor driver vars
//called this way, it uses the default address 0x40
Adafruit_PWMServoDriver board = Adafruit_PWMServoDriver(0x40);

//setup wifi
void setup_wifi() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  
 if(WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Couldn't connect to WiFi.");
      while(1) delay(100);
  }
 
  Serial.print("Connected to the WiFi network: ");
  Serial.println(ssid);
}

//setup mqtt
void setup_mqtt() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}

//setup adafruit board
void setup_adafruit_board() {
  board.begin();
  board.setPWMFreq(60);
}

//reconnect to mqtt
boolean reconnect() {
  if (client.connect(client_id)) {
    client.subscribe(topic_input); // Subscribe to channel.
  }
  return client.connected();
}

//move body joint (servo motor)
void moveJoint(int joint, int degree) {
  int sign = 1;
  if(joint==NECK || joint==LEFT_FRONT_ARM || joint==RIGHT_BACK_ARM || joint==RIGHT_FRONT_SHOULDER || joint==RIGHT_BACK_SHOULDER){
    sign = -1;
  }
  //Serial.print("Move Joint ");
  //Serial.print(joint);
  //Serial.print(" ");
  //Serial.println(degree);
  int pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, SERVOMIN, SERVOMAX);
  board.setPWM(joint, 0, pulse);
}

//mqtt message callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String payload_buff;
  for (int i=0;i<length;i++) {
    payload_buff = payload_buff+String((char)payload[i]);
  }
  Serial.println(payload_buff);

  //relax servos lite
  if(payload_buff == "relax") {
    for(int i=0; i<10; i++){  
      moveJoint(i, 0);  
    }
    return;
  }
  
  //test servos lite
  if(payload_buff == "testservo") {
    for(int i=0; i<10; i++){  
      moveJoint(i, 0);  
    }
  	for(int i=0; i<10; i++){  
  		moveJoint(i, 0);  
  		delay(1000);		
  		moveJoint(i, -20);  
  		delay(1000);	
  		moveJoint(i, 0);  
  		delay(1000);	
  		moveJoint(i, +20);  
  		delay(1000);	
  	  moveJoint(i, 0);  
      delay(1000);  
    }
  	return;
  }
  
  //test servos full
  if(payload_buff == "testservofull") {
  	for(int i=0; i<10; i++){      
  		for(int angle=-90; angle<=90; angle+=10){
  			moveJoint(i, angle);
  			delay(1000);
  		}
    }
  	return;
  }
  
  //test led
  if(payload_buff == "testledOn") {
    digitalWrite(BUILTIN_LED, LOW);
	  return;
  }
  if(payload_buff == "testledOff") {
    digitalWrite(BUILTIN_LED, HIGH);
	  return;
  }
  
  //get json payload
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload_buff);
  JsonObject root = doc.to<JsonObject>();
  
  //translate json to servo moves
  if(root.containsKey("HEAD")) {
	  int degree = root["HEAD"].as<int>();
	  moveJoint(HEAD, degree);
  }
  if(root.containsKey("NECK")) {
	  int degree = root["NECK"].as<int>();
	  moveJoint(NECK, degree);
  }
  if(root.containsKey("RIGHT_FRONT_SHOULDER")) {
	  int degree = root["RIGHT_FRONT_SHOULDER"].as<int>();
	  moveJoint(RIGHT_FRONT_SHOULDER, degree);
  }
  if(root.containsKey("RIGHT_FRONT_ARM")) {
	  int degree = root["RIGHT_FRONT_ARM"].as<int>();
	  moveJoint(RIGHT_FRONT_ARM, degree);
  }
  if(root.containsKey("LEFT_FRONT_SHOULDER")) {
	  int degree = root["LEFT_FRONT_SHOULDER"].as<int>();
	  moveJoint(LEFT_FRONT_SHOULDER, degree);
  }
  if(root.containsKey("LEFT_FRONT_ARM")) {
	int degree = root["LEFT_FRONT_ARM"].as<int>();
	moveJoint(LEFT_FRONT_ARM, degree);
  }
  if(root.containsKey("RIGHT_BACK_SHOULDER")) {
	  int degree = root["RIGHT_BACK_SHOULDER"].as<int>();
	  moveJoint(RIGHT_BACK_SHOULDER, degree);
  }
  if(root.containsKey("RIGHT_BACK_ARM")) {
	  int degree = root["RIGHT_BACK_ARM"].as<int>();
	  moveJoint(RIGHT_BACK_ARM, degree);
  }
  if(root.containsKey("LEFT_BACK_SHOULDER")) {
	  int degree = root["LEFT_BACK_SHOULDER"].as<int>();
	  moveJoint(LEFT_BACK_SHOULDER, degree);
  }
  if(root.containsKey("LEFT_BACK_ARM")) {
	  int degree = root["LEFT_BACK_ARM"].as<int>();
	  moveJoint(LEFT_BACK_ARM, degree);
  }
  
  //send feedback message
  /*String output;
  serializeJson(doc, output);
  char output_char[output.length()];
  output.toCharArray(output_char, output.length());
  Serial.println("Sending message to MQTT topic..");
  Serial.println(output);
  if (client.publish(topic_output, output_char) == true) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }*/
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  setup_mqtt();
  setup_adafruit_board();
}

void loop() {
  //loop for mqtt
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) { // Try to reconnect.
      lastReconnectAttempt = now;
      if (reconnect()) { // Attempt to reconnect.
        lastReconnectAttempt = 0;
      }
    }
  } 
  else { // Connected.
    client.loop();
    //client.publish(topic_output, "Hello world!"); // Publish message.
    delay(1000);
  }
}
