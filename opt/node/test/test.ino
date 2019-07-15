#include <SPI.h>
#include <LoRa.h>
#include "ovo.h"

int counter = 0;
int lastTime=0;
int ledPin = 6;
//int interv = 500;
char* input="0";

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  //Serial.println("LoRa 2 way comm (control LED and send data)");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,HIGH);
  delay(5);
  digitalWrite(ledPin,LOW);
  //LoRa.onReceive(gotPacket);
  //LoRa.dumpRegisters(Serial);
}

void loop() {
  

  //Serial.print("Sending packet: ");
  //Serial.println(counter);

 // unsigned long currentTime = millis();


 
  //if(currentTime - lastTime > interv) {

  //  lastTime = currentTime;   
    int aVal=analogRead(A0);
   // Serial.print("\nA: ");
    //Serial.print(aVal);
  // send packet
    /*LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.print("  ");
    LoRa.print(aVal);
    LoRa.endPacket();
    delay(200);//wait for SPI to do its thing, maybe
    counter++;*/
  //}
  //Serial.println(counter);
  
  int packetSize = LoRa.parsePacket();
  //Serial.print(packetSize);
  if (packetSize) {
   gotPacket(packetSize);
  }
    LoRa.idle();
    delay(1000);

    if(LoRa.beginPacket()){
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.print("  ");
    LoRa.print(aVal);
    LoRa.endPacket();
    Serial.println("sent");
    }
delay(3000);
  

  //delay(500);
}

void gotPacket(int packetSize)
{
   char inputBuff[999];
    int i=0;
    // received a packet
    Serial.print("Received packet :  ");

    // read packet
       while (LoRa.available()) {
      
    //  Serial.print((char)LoRa.read());
      inputBuff[i]=(char)LoRa.read();
      i++;
    }
    inputBuff[i]='\0';
   Serial.println(inputBuff);
  
  
  
  }
