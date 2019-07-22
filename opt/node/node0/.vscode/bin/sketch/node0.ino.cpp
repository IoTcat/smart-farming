#include <Arduino.h>
#line 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
#line 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"

#define LORA_SOCKET_IP "1.0.0.1"
#include "lora-mqtt.h"

LoRaMQTT mqtt;

void(* resetFunc) (void) = 0;

#line 9 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void setup();
#line 32 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void loop();
#line 38 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void mqttRes(String subject, String content);
#line 58 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void getData(String& s, const String& content);
#line 79 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
bool pumpOn();
#line 85 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
bool pumpOff();
#line 9 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void setup() {

    Serial.begin(115200);

    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(4, OUTPUT);

    mqtt.ini();
    mqtt.subscribe("#");
    mqtt.onReceived(mqttRes);

    Serial.println("started..");

}

void loop() {

    if(millis() > 180000) resetFunc();
    mqtt.core();
}

void mqttRes(String subject, String content){
  Serial.println("res");
  if(subject == "qos/sync" && content.toInt()>=100 && content.toInt()<200){
    String s;
    getData(s, content);
    Serial.println(s);
    mqtt.publish("res/json", s);
  }
  if(subject == "ctl/node0/waterSwitch"){
    if(content == "0") pumpOn();
    if(content == "1") pumpOff();
  }

    delay(1000);
    resetFunc();


}


void getData(String& s, const String& content){
  s = "{\"t\":\"node0\",\"b\":";
  s += ((long int)analogRead(A2)*100)/1024;
  s += ",\"w\":";
  s += (analogRead(A3) > 50) ? 0 : 1;
  s += ",\"T\":";
  s += (125*analogRead(A1))>>8;
  s += ",\"h\":";
  s += analogRead(A0);
  s += ",\"s\":";
  s += 0;
  s += ",\"q\":";
  s += content;
  s += "}";

}





bool pumpOn(){
  digitalWrite(4, HIGH);
  Serial.println("pumpOn");
  return true;
}

bool pumpOff(){
  digitalWrite(4, LOW);
  Serial.println("pumpOff");
  return true;
}


