//https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
//https://robojax.com/learn/arduino/?vid=robojax_PCA9685-V4
//https://arduinojson.org/v6/doc/installation/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

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

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver board = Adafruit_PWMServoDriver(0x40);

//configuration
const char* ssid     = "Vodafone-C01960075";
const char* password = "tgYsZkgHA4xhJLGy";
const char* mqtt_server = "test.mosquitto.org";
const char* topic_input = "myhome/mx/cserver";
const char* topic_output = "myhome/mx/cserver";

WiFiClient espClient;
PubSubClient client(espClient);

//move body joint
void moveJoint(joint, degree) {
    sign = 1
    if(joint==NECK || joint==LEFT_FRONT_ARM || joint==RIGHT_BACK_ARM || joint==RIGHT_FRONT_SHOULDER || joint==RIGHT_BACK_SHOULDER]){
        sign = -1;
	}
	int pulse = map(degree*sign, DEGREEMIN, DEGREEMAX, SERVOMIN, SERVOMAX);
    board.setPWM(joint, 0, pulse);
}

//setup wifi
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Servoboard init!");
  board.begin();
  board.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
 
}

//mqtt message callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  //test servos
  char json[] = (char)payload[0];
  if((char)payload[0] == 'testservo') {
	for(int angle=0; angle<181; angle +=10){
		for(int i=0; i<10; i++){      
            moveJoint(i, angle);
        }
    }
	return;
  }
  
  //test led
  if((char)payload[0] == 'testledOn') {
    digitalWrite(BUILTIN_LED, LOW);
	return;
  }
  if((char)payload[0] == 'testledOff') {
    digitalWrite(BUILTIN_LED, HIGH);
	return;
  }
  
  //get json payload
  char json[] = (char)payload[0];
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);
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
  
  //send message
  char JSONmessageBuffer[200];
  root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);
  if (client.publish(topic_output, JSONmessageBuffer) == true) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }
}

//reconnect to mqtt
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic_input, "hello world");
      // ... and resubscribe
      client.subscribe(topic_input);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}