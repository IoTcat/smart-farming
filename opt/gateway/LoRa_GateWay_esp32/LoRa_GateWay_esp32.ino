#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

#define LORA_SOCKET_IP "1.0.0.1"

#include "lora-mqtt.h"


#define MQTT_MAX_TRANSFER_SIZE 99999

#include <PubSubClient.h>
#include <U8x8lib.h>
#include "ovo.h"


// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define Band 433E6

//#define syncWord 0xF3

//============================
//CHANGE THIS FOR EACH ARDUINO
String nodeId ="Gateway";
//============================

const char* ssid = "yimian-iot";
const char* password =  "1234567890.";
const char* mqtt_server = "192.168.1.102";//change this to the mqtt server

char* topicIn="#";//change this to the outgoing messages
String cache_mqtt_publish = "";
String cache_mqtt_subject = "";


WiFiClient espClient;
PubSubClient client(espClient);

LoRaMQTT mqtt;

char outMsg[200];
long lastMsg = 0;

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void setup() {
  Serial.begin(115200);
  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  LoRa.setPins(SS, RST, DI0);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); 
    u8x8.drawString(0, 6, "cnn to WiFi");
  
  }
 

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 30030);//default port, change it...
  client.setCallback(callback);

Serial.println("LoRa Receiver");

  if (!LoRa.begin(Band)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 6, "LoRa failure");
  }

  mqtt.ini();
  mqtt.subscribe("#");
  mqtt.onReceived(mqttRes);
  
  Serial.println("Lora Started...\n");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  u8x8.drawString(0, 6, "waiting..");
  u8x8.drawString(0,0,"X)"); 
}



void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String s = "";
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    s += (char)payload[i];
  }
  Serial.println("");

  mqtt.publish(String(topic), s);

  //send to LoRa nodes..
  //sendToNode();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
        
    String clientId = nodeId;    
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(topicOut, "Hello from the Gateway!");
      // ... and resubscribe
      client.subscribe(topicIn);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 3 seconds before retrying
      delay(3000);
    }
  }
}



void f(){
  
  if(cache_mqtt_publish != "")  {

    client.publish(cache_mqtt_subject.c_str(), cache_mqtt_publish.c_str(), cache_mqtt_publish.length());
    cache_mqtt_publish = "";
    cache_mqtt_subject = "";
  }
}

void mqttRes(String subject, String content){
  Serial.println("LoRaMQTT::"+subject+"::"+content);
  cache_mqtt_subject = subject;
  cache_mqtt_publish = content;

}


void loop() {

  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  mqtt.core();
  f();
}

