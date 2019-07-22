//#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include "BME280.h"
#include "GY30.h"

//const size_t capacity = JSON_OBJECT_SIZE(15);
//DynamicJsonDocument data(capacity);

#define LORA_SOCKET_IP "1.0.0.0"
#include "lora-mqtt.h"

LoRaMQTT mqtt;


BME280 bme;
GY30 gy;

void (* resetFunc)(void) = 0;

void setup() {
    uint8_t osrs_t = 1;             //Temperature oversampling x 1
    uint8_t osrs_p = 1;             //Pressure oversampling x 1
    uint8_t osrs_h = 1;             //Humidity oversampling x 1
    uint8_t mode = 3;               //Normal mode
    uint8_t t_sb = 5;               //Tstandby 1000ms
    uint8_t filter = 0;             //Filter off 
    uint8_t spi3w_en = 0;           //3-wire SPI Disable
    
    uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
    uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
    uint8_t ctrl_hum_reg  = osrs_h;
    
    Serial.begin(115200);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A3, INPUT);
    pinMode(A7, INPUT);

    Wire.begin();
    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

    mqtt.ini();
    mqtt.subscribe("#");
    mqtt.onReceived(mqttRes);
    
    bme.writeReg(0xF2,ctrl_hum_reg);
    bme.writeReg(0xF4,ctrl_meas_reg);
    bme.writeReg(0xF5,config_reg);
    bme.readTrim();

    Serial.println("started..");
}

void loop() {

    if(millis() > 180000) resetFunc();
    mqtt.core();

}

void mqttRes(String subject, String content){
  if(subject == "qos/sync" && content.toInt()>=0 && content.toInt()<100){
    String s;
    getData(s, content);
    //data = getJson();
    //data["qos"] = content;
    //data["type"] = "station";
    //serializeJson(data, dataStr);
    Serial.println(s);
    mqtt.publish("res/json", s);

  }
    delay(1000);
    resetFunc();
}


void getData(String& s, const String& content){
    s = "{\"t\":\"station\",\"l\": ";
    s += gy.getLight();
    s += ",\"T\":";
    s += bme.getTemp();
    s += ",\"h\":";
    s += bme.getHum();
    s += ",\"a\":";
    s += bme.getPress()/100;
    s += ",\"C\":";
    s += analogRead(A0);
    s += ",\"N\":";
    s += analogRead(A1);
    s += ",\"r\":";
    s += analogRead(A2);
    s += ",\"b\":";
    s += (unsigned int)analogRead(A6)*100/1024;
    s += ",\"s\":";
    s += 0;
    s += ",\"q\":";
    s += content;
    s += "}";

}


/* 
//DynamicJsonDocument createJson(const size_t capacity, int Light, long int batteryLevel, double Temp, double Hum, double Press, int CO, int NH3, int Rain)
DynamicJsonDocument getJson()
{
  DynamicJsonDocument json(capacity);
  int Light = gy.getLight();
  double Temp = bme.getTemp();
  double Hum = bme.getHum();
  double Press = bme.getPress()/100;
  int CO = analogRead(A0);
  int NH3 = analogRead(A1);
  int Rain = analogRead(A2);
  long int batteryLevel = analogRead(A6);
  batteryLevel = (batteryLevel * 100)/1024;
  
    Serial.print("Light: ");
    Serial.println(Light);
    Serial.print("batteryLevel: ");
    Serial.println(batteryLevel);
    Serial.print("Temperature: ");
    Serial.println(Temp);
    Serial.print("Humidity: ");
    Serial.println(Hum);
    Serial.print("Press: ");
    Serial.println(Press);
    Serial.print("Rain: ");
    Serial.println(Rain);
    Serial.print("CO: ");
    Serial.println(CO);
    Serial.print("NH3: ");
    Serial.println(NH3);
    Serial.println();
    
  json["status"] = "0";
  json["batterylevel"] = batteryLevel;
  json["light"] = Light;
  json["temperature"] = Temp;
  json["humidity"] = Hum;
  json["rainfall"] = Rain;
  json["co"] = CO;
  json["nh3"] = NH3;
  json["airpressure"] = Press;
  return json;

}


*/