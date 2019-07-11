#include <SPI.h>
#include <LoRa.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <U8x8lib.h>

//#include<ArduinoJson.h>

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

#define syncWord 0xF3

//============================
//CHANGE THIS FOR EACH ARDUINO
char* nodeId ="Gateway";
//============================

const char* ssid = "yimian-iot";
const char* password =  "1234567890.";
const char* mqtt_server = "192.168.4.31";//change this to the mqtt server
char* topicOut="test/esp";//change this to the incoming messages
char* topicIn="#";//change this to the outgoing messages

WiFiClient espClient;
PubSubClient client(espClient);

char msg[200];
char outMsg[200];
//long lastMsg = 0;

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

  LoRa.setSyncWord(syncWord);//connect to only our network
  
  LoRa.onReceive(gotMessage);// set the interrupt function
  LoRa.receive();// put LoRa in Receive mode
  
  Serial.println("Lora Started...\n");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  u8x8.drawString(0, 6, "waiting..");
  u8x8.drawString(0,0,"X)"); 
}



void gotMessage(int packetSize)
{
  if(packetSize==0)
  return;

  int i=0;
  //int recipient=LoRa.read();//recipeint address
  //byte sender=LoRa.read();//sender address
  //byte messageId=LoRa.read();//message ID
  //byte msgLength=LoRa.read();

  while(LoRa.available())
  {
    msg[i]=LoRa.read();
    i++;  
  }
  u8x8.drawString(0, 6, "msg:");
  
  msg[i]='\0';
  //send to MQTT here...
  u8x8.drawString(0, 8, msg);
  client.publish(topicOut, msg);
  
}


void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    outMsg[i]=(char)payload[i];
  }
  u8x8.drawString(0, 8, outMsg);
  //send to LoRa nodes..
  sendToNode(outMsg);

}


void sendToNode(char* message)
{
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    
    String clientId = "NodeID: gateway1";//+nodeId;    
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topicOut, "Hello from the Gateway!");
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




void loop() {

  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

}
