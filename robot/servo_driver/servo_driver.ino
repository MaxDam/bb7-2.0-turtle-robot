//Preparazione IDE ESP32:
//File > Preferences > Additional Board Manager URLs > https://dl.espressif.com/dl/package_esp32_index.json
//Tools > Board Arduino Uno > Boards Manager > ESP32
//Tools > Board Arduino Uno > ESP32 Arduino" > DOIT ESP32 DEVKIT V1
//Tools > Manage Libraries > PubSubClient
//Tools > Manage Libraries > Adafruit PWM Servo Driver Library by Adafruit
//Tools > Port > <Selezionare porta seriale disponibile>

//links:
//https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
//https://techtutorialsx.com/2017/04/24/esp32-connecting-to-a-wifi-network/
//https://learn.adafruit.com/16-channel-pwm-servo-driver/using-the-adafruit-library
//https://robojax.com/learn/arduino/?vid=robojax_PCA9685-V4
//https://www.pubnub.com/blog/pubsub-nodemcu-32s-esp32-mqtt-pubnub-arduino-sdk/
//https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//servo range
#define DEGREEMIN -90 //'minimum' degree
#define DEGREEMAX 90  //'maximum' degree

//body joint
#define HEAD 				          0
#define NECK 				          1
#define RIGHT_FRONT_SHOULDER  2
#define RIGHT_FRONT_ARM		    3
#define LEFT_FRONT_SHOULDER	  4
#define LEFT_FRONT_ARM	      5
#define RIGHT_BACK_SHOULDER	  6
#define RIGHT_BACK_ARM 		    7
#define LEFT_BACK_SHOULDER 	  8
#define LEFT_BACK_ARM	        9

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

  //degree limit
  if(degree > DEGREEMAX) degree = DEGREEMAX;
  if(degree < DEGREEMIN) degree = DEGREEMIN;

  int pulse = 0;
  
  if(joint==HEAD){
    //center=430
    int servoMin = 270;
    int servoMax = 590;
    int sign = -1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==NECK){
    //center=420
    int servoMin = 240;
    int servoMax = 600;
    int sign = -1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==RIGHT_FRONT_SHOULDER){
    //center=420
    int servoMin = 320;
    int servoMax = 520;
    int sign = -1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==RIGHT_FRONT_ARM){
    //center=475
    int servoMin = 350;
    int servoMax = 600;
    int sign = 1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==LEFT_FRONT_SHOULDER){
    //center=460
    int servoMin = 360;
    int servoMax = 560;
    int sign = 1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==LEFT_FRONT_ARM){
    //center=390
    int servoMin = 300;
    int servoMax = 480;
    int sign = -1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==RIGHT_BACK_SHOULDER){
    //center=420
    int servoMin = 320;
    int servoMax = 520;
    int sign = -1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==RIGHT_BACK_ARM){
    //center=370
    int servoMin = 260;
    int servoMax = 480;
    int sign = -1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==LEFT_BACK_SHOULDER){
    //center=490
    int servoMin = 390;
    int servoMax = 590;
    int sign = 1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }
  if(joint==LEFT_BACK_ARM){
    //center=420
    int servoMin = 340;
    int servoMax = 500;
    int sign = 1;
    pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, servoMin, servoMax);
  }

  //debug
  if(true) {
    Serial.print("Move: Joint ");
    Serial.print(joint);
    Serial.print(" degree: ");
    Serial.print(degree);
    Serial.print(" pulse: ");
    Serial.println(pulse);
  }

  //move servo
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

  //relax servos
 if(payload_buff.startsWith("relax")) {
    for(int i=0; i<10; i++){  
      moveJoint(i, 0);  
    }
    return;
  }
  
  //test servos
  if(payload_buff.startsWith("test")) {
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
  
  //led
  if(payload_buff.startsWith("led")) {
    int onoff = getValue(payload_buff, ':', 1).toInt();
    digitalWrite(BUILTIN_LED, (onoff ? HIGH : LOW));
	  return;
  }

  //setup servo
  //setup|joint:pulse|joint:pulse|.. (ex. setup:0=100:2=388)
  if(payload_buff.startsWith("setup")) {
    for(int i = 0; i <= 10; i++) {
      String token = getValue(payload_buff, ':', i);
      if(token!="setup" && token!="") {
        int joint = getValue(token, '=', 0).toInt();
        int pulse = getValue(token, '=', 1).toInt();
        board.setPWM(joint, 0, pulse);
      }
    }
    return;
  }

  //move servo
  //move|joint:degree|joint:degree|.. (ex. move:0=30:2=-70)
  if(payload_buff.startsWith("move")) {
    for(int i = 0; i <= 10; i++) {
      String token = getValue(payload_buff, ':', i);
      if(token!="move" && token!="") {
        int joint = getValue(token, '=', 0).toInt();
        int degree = getValue(token, '=', 1).toInt();
        moveJoint(joint, degree);
      }
    }
  }

  //send feedback message
  client.publish(topic_output, payload_buff.c_str());
}

//get values from separated token string
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
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
