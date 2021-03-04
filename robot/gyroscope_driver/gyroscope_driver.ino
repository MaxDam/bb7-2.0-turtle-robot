//IDE ESP8266
//https://www.antoniovasco.it/2018/08/11/configurare-wemos-d1-mini-con-arduino-ide/
//File > Preferences > Additional Board Manager URLs > http://arduino.esp8266.com/stable/package_esp8266com_index.json
//Board > Boards Manager > ESP8266
//Tools > Board: "LOLIN(WEMOS) D1 mini Lite" > LOLIN(WEMOS) D1 mini Lite
//Tools > Port > Selezionare porta com

//MAG3110 3-axis Sensor Module Electronic Compass Board
//http://www.esp8266learning.com/l3g4200d-three-axis-gyroscope-and-esp8266-example.php

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <L3G4200D.h>
 
#define MAG_ADDR  0x0E //7-bit address for the MAG3110, doesn't change
 
//configuration
const char* ssid     = "Vodafone-C01960075";
const char* password = "tgYsZkgHA4xhJLGy";
const char* mqtt_server = "test.mosquitto.org";
const char* topic_input = "myhome/mx/cserver";
const char* topic_output = "myhome/mx/cserver";

WiFiClient espClient;
PubSubClient client(espClient);
L3G4200D gyroscope;

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

void setupGyro() 
{
  Serial.begin(9600);
  Serial.println("Initialize L3G4200D");
 
  while(!gyroscope.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50))
  {
    Serial.println("Could not find a valid L3G4200D sensor, check wiring!");
    delay(500);
  }
 
  // Check selected scale
  Serial.print("Selected scale: ");
 
  switch(gyroscope.getScale())
  {
    case L3G4200D_SCALE_250DPS:
      Serial.println("250 dps");
      break;
    case L3G4200D_SCALE_500DPS:
      Serial.println("500 dps");
      break;
    case L3G4200D_SCALE_2000DPS:
      Serial.println("2000 dps");
      break;
    default:
      Serial.println("unknown");
      break;
  }
 
  // Check Output Data Rate and Bandwidth
  Serial.print("Output Data Rate: ");
 
  switch(gyroscope.getOdrBw())
  {
    case L3G4200D_DATARATE_800HZ_110:
      Serial.println("800HZ, Cut-off 110");
      break;
    case L3G4200D_DATARATE_800HZ_50:
      Serial.println("800HZ, Cut-off 50");
      break;
    case L3G4200D_DATARATE_800HZ_35:
      Serial.println("800HZ, Cut-off 35");
      break;
    case L3G4200D_DATARATE_800HZ_30:
      Serial.println("800HZ, Cut-off 30");
      break;
    case L3G4200D_DATARATE_400HZ_110:
      Serial.println("400HZ, Cut-off 110");
      break;
    case L3G4200D_DATARATE_400HZ_50:
      Serial.println("400HZ, Cut-off 50");
      break;
    case L3G4200D_DATARATE_400HZ_25:
      Serial.println("400HZ, Cut-off 25");
      break;
    case L3G4200D_DATARATE_400HZ_20:
      Serial.println("400HZ, Cut-off 20");
      break;
    case L3G4200D_DATARATE_200HZ_70:
      Serial.println("200HZ, Cut-off 70");
      break;
    case L3G4200D_DATARATE_200HZ_50:
      Serial.println("200HZ, Cut-off 50");
      break;
    case L3G4200D_DATARATE_200HZ_25:
      Serial.println("200HZ, Cut-off 25");
      break;
    case L3G4200D_DATARATE_200HZ_12_5:
      Serial.println("200HZ, Cut-off 12.5");
      break;
    case L3G4200D_DATARATE_100HZ_25:
      Serial.println("100HZ, Cut-off 25");
      break;
    case L3G4200D_DATARATE_100HZ_12_5:
      Serial.println("100HZ, Cut-off 12.5");
      break;
    default:
      Serial.println("unknown");
      break;
  }
 
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  gyroscope.calibrate();
 
  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  gyroscope.setThreshold(3);
}

 
void send_gyro_values(void)
{
  // Read normalized values
  Vector raw = gyroscope.readRaw();
 
  // Read normalized values in deg/sec
  Vector norm = gyroscope.readNormalize();
 
  //acquire info and send message
  const char* value = "Xraw =" + raw.XAxis + ", Yraw = " + norm.YAxis + ", ZNorm = " +norm.ZAxis;
  snprintf(msg, MSG_BUFFER_SIZE, value);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(topic_output, msg);
}

void setup()
{
  Wire.begin();          // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  setupGyro(); 			 //setup Gyroscope
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  send_gyro_values();
  delay(5);
}

