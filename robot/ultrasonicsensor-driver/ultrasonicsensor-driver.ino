//IDE ESP8266
//https://www.antoniovasco.it/2018/08/11/configurare-wemos-d1-mini-con-arduino-ide/
//File > Preferences > Additional Board Manager URLs > http://arduino.esp8266.com/stable/package_esp8266com_index.json
//Board > Boards Manager > ESP8266
//Tools > Board: "LOLIN(WEMOS) D1 mini Lite" > LOLIN(WEMOS) D1 mini Lite
//Tools > Port > Selezionare porta com

//HC-SR04 ultrasonic sensor
//http://www.esp8266learning.com/wemos-mini-hc-sr04-ultrasonic-sensor.php

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
 
#define echoPin D7 // Echo Pin
#define trigPin D6 // Trigger Pin
 
//configuration
const char* ssid     = "Vodafone-C01960075";
const char* password = "tgYsZkgHA4xhJLGy";
const char* mqtt_server = "test.mosquitto.org";
const char* topic_input = "myhome/mx/cserver";
const char* topic_output = "myhome/mx/cserver";

WiFiClient espClient;
PubSubClient client(espClient);

long duration, distance; // Duration used to calculate distance

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

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
  
  //test led
  if((char)payload[0] == 'testledOn') {
    digitalWrite(BUILTIN_LED, LOW);
	return;
  }
  if((char)payload[0] == 'testledOff') {
    digitalWrite(BUILTIN_LED, HIGH);
	return;
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

//setup sensor
void setupUltrasonicSensor() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

//send distance value
void send_distance_value(void)
{
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  Serial.println(distance);
 
  //acquire info and send message
  snprintf(msg, MSG_BUFFER_SIZE, distance);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(topic_output, msg);
}

void setup()
{
  Wire.begin();          // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  
  setupUltrasonicSensor();
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  send_distance_value();
  delay(1000);
}
