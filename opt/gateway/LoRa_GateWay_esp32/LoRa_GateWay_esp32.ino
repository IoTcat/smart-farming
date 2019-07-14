#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <U8x8lib.h>
//#include "ovo.h"

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

//#define syncWord 0xF3

//============================
//CHANGE THIS FOR EACH ARDUINO
char nodeId[15] ="Gateway";
//============================

const char* ssid = "yimian-iot";
const char* password =  "1234567890.";
const char* mqtt_server = "192.168.4.31";//change this to the mqtt server

char* topicIn="qos/sync";//change this to the outgoing messages

//char* jsonData;

//const int capacity = JSON_OBJECT_SIZE(14);//station has 14 objects, its the max amount
//StaticJsonDocument<capacity> jb;

WiFiClient espClient;
PubSubClient client(espClient);

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
 //LoRa_rxMode(); LoRa.receive();// put LoRa in Receive mode
  //sendToNode();
  //delay(500);
  //sendToNode();
  //delay(500);
  //sendToNode();
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

  char msg[999];

 //String buff;
   //DynamicJsonBuffer jBuffer;
   //JsonObject& root = jBuffer.createObject();
   

  while(LoRa.available())
  {
    msg[i]=LoRa.read();
    i++;  
  }
  msg[i] = '\0';

    Serial.println(msg);

  //sendToNode();
  
/*  
  //JsonObject& obj =  jb.parseObject(msg);
  //5.13.1
  DeserializationError err = deserializeJson(jb,msg);
  if(err)
  {
      u8x8.drawString(0, 6, "parse_err");// parse error
      return;
   }

  
  const char* type=jb["type"];
  char* topicOut;
  //change to string if it doesnt work
  u8x8.drawString(0, 6, "from:");// who are we getting messages from?
  u8x8.drawString(0, 8, type); //show on LCD

  if(type == "node" )
  {

    int nodeId=jb["id"];
    int stat=jb["status"];
    int batLev=jb["batteryLevel"];
    const char* watTemp=jb["temperature"];
    int qos = jb["qos"];
    int switchState=jb["waterSwitch"];
    const char* humid=jb["humidity"];
    
    switch(nodeId)
    {
      case 0:
               
       topicOut="res/node0/status";
       client.publish(topicOut, ""+stat);
       
       topicOut="res/node0/batteryLevel";
       client.publish(topicOut, ""+batLev);
       
       topicOut="res/node0/waterSwitch";
       client.publish(topicOut,""+switchState);

       topicOut="res/node0/temperature";
       client.publish(topicOut, watTemp);

       topicOut="res/node0/humidity";
       client.publish(topicOut,humid);
              
       topicOut="qos/node0";
       client.publish(topicOut,""+qos);
       break;
       
      case 1:
              
        topicOut="res/node1/status";
        client.publish(topicOut, ""+stat);

        topicOut="res/node1/batteryLevel";
        client.publish(topicOut, ""+batLev);       
     
        topicOut="res/node1/waterSwitch";
        client.publish(topicOut,""+switchState);
    
        topicOut="res/node1/temperature";
        client.publish(topicOut, watTemp);

        topicOut="res/node1/humidity";
        client.publish(topicOut, humid);
       
        topicOut="qos/node1";
        client.publish(topicOut,""+qos);   
        
        break;
        
      default:
        u8x8.drawString(0, 8, "no ID =("); //show on LCD
        break;
      }
        
    }
    
  else if(type =="station") 
  {

    int stat =jb["status"];
    int batLev=jb["batterylevel"];
    const char* light =jb["light"];
    const char* temp=jb["temperature"];
    const char* humid =jb["humidity"];
    int rainF=jb["rainfall"];
    int co =jb["co"];
    int nh3 =jb["nh3"];    
    const char* airP =jb["airpressure"];
    int qos=jb["qos"];
    
      topicOut="res/station/status";  
      client.publish(topicOut, ""+stat);
      
      topicOut="res/station/batteryLevel";  
      client.publish(topicOut, ""+batLev);
      
      topicOut="res/station/light";   
      client.publish(topicOut,light );
            
      topicOut="res/station/temperature"; 
      client.publish(topicOut,temp );
      
      topicOut="res/station/humidity";  
      client.publish(topicOut, humid);
      
      topicOut="res/station/rainfall"; 
      client.publish(topicOut, ""+rainF );
      
      topicOut="res/station/CO"; 
      client.publish(topicOut, ""+co);
            
      topicOut="res/station/NH3";
      client.publish(topicOut, ""+nh3);
      
      topicOut="res/station/airPressure";
     // client.publish(topicOut, airP);
 
      topicOut="qos/station";
      client.publish(topicOut,""+qos);      
  }
  else
  {
      u8x8.drawString(0, 8, "JSON ERR"); //show on LCD
  }
  
  //send to MQTT here...
 
  client.publish(topicOut, msg);
  
   msg[0]='\0';
   jb.clear();//clear memory for next message
*/   

   
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
  
  //send to LoRa nodes..
  sendToNode();

}

void LoRa_rxMode(){
  //LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  //LoRa.enableInvertIQ();                // active invert I and Q signals
}


void sendToNode()
{
  LoRa_txMode();                        // set tx mode
  delay(200);
  LoRa.beginPacket();                   // start packet
  LoRa.print("lalala");                  // add payload
  LoRa.endPacket();                     // finish packet and send it
  delay(200);
  LoRa_rxMode();
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




void loop() {

  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

}

