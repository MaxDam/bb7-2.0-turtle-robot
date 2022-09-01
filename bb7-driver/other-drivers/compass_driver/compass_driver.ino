//IDE ESP8266
//https://www.antoniovasco.it/2018/08/11/configurare-wemos-d1-mini-con-arduino-ide/
//File > Preferences > Additional Board Manager URLs > http://arduino.esp8266.com/stable/package_esp8266com_index.json
//Board > Boards Manager > ESP8266
//Tools > Board: "LOLIN(WEMOS) D1 mini Lite" > LOLIN(WEMOS) D1 mini Lite
//Tools > Port > Selezionare porta com

//MAG3110 3-axis Sensor Module Electronic Compass Board
//https://www.esp8266learning.com/wemos-mag3110-magnetometer.php

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
 
#define MAG_ADDR  0x0E //7-bit address for the MAG3110, doesn't change
 
//configuration
const char* ssid     = "Vodafone-C01960075";
const char* password = "tgYsZkgHA4xhJLGy";
const char* mqtt_server = "test.mosquitto.org";
const char* topic_input = "myhome/mx/cserver";
const char* topic_output = "myhome/mx/cserver";

WiFiClient espClient;
PubSubClient client(espClient);

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

//setup compass
void setupCompass(void)
{
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x11);             // cntrl register2
  Wire.write(0x80);             // write 0x80, enable auto resets
  Wire.endTransmission();       // stop transmitting
  delay(15); 
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x10);             // cntrl register1
  Wire.write(1);                // write 0x01, active mode
  Wire.endTransmission();       // stop transmitting
}
 
int mag_read_register(int reg)
{
  int reg_val;
 
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(reg);              // x MSB reg
  Wire.endTransmission();       // stop transmitting
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
 
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may write less than requested
  { 
    reg_val = Wire.read(); // read the byte
  }
 
  return reg_val;
}
 
int mag_read_value(int msb_reg, int lsb_reg)
{
  int val_low, val_high;  //define the MSB and LSB
  val_high = mag_read_register(msb_reg);
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  val_low = mag_read_register(lsb_reg);
  int out = (val_low|(val_high << 8)); //concatenate the MSB and LSB
  return out;
}
 
int read_x(void)
{
  return mag_read_value(0x01, 0x02);
}
 
int read_y(void)
{
  return mag_read_value(0x03, 0x04);
}
 
int read_z(void)
{
  return mag_read_value(0x05, 0x06);
}

void send_compass_values(void)
{
  const char* value = "x=" + read_x() + ",y=" + read_y() + ",z=" +read_z();
  snprintf(msg, MSG_BUFFER_SIZE, value);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(topic_output, msg);
}
 
void setup()
{
  Wire.begin();          // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  setupCompass();        // turn the MAG3110 on
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  send_compass_values();
  delay(1000);
}
