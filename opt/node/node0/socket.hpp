#ifndef __SOCKET_HPP_
#define __SOCKET_HPP_

#include <LoRa.h>
#include <SPI.h>
#include "ovo.h"
#include "vector.h"

#ifndef SS
#define SS 18
#endif
#ifndef RST
#define RST 14
#endif
#ifndef DI0
#define DI0 26
#endif
#ifndef BAND
#define BAND 433E6
#endif
#ifndef SEND_TIMES
#define SEND_TIMES 4
#endif
#ifndef SEND_INTERVAL
#define SEND_INTERVAL 20
#endif
#ifndef TCP_TIMEOUT
#define TCP_TIMEOUT 1000
#endif


#define DEFAULT_SERIAL 115200


char inputByte = 1;

class Socket {
   public:
    void core();
    void ini();
    void onReceive(auto handler);
    void udp(const String& msg, auto callback);
    inline void udp(const String& msg) {
        this->udp(msg, [=](int err) {});
    };

   private:
    Vector<int> udp_sendingStack;
    void LoRa_rxMode();
    void LoRa_txMode();
    void LoRa_sendMessage(const String& message);

};

void Socket::ini() {

    if(!Serial) Serial.begin(DEFAULT_SERIAL);

    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while(true);
    }
}

void Socket::core() {
    int packetSize = LoRa.parsePacket();
    // Serial.print(packetSize);
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRa.available()) {
            Serial.print((char)LoRa.read());
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }

    if (Serial.available() > 0) {
        inputByte = Serial.read();

        // package it and send off
        // Serial.println(inputByte);
        LoRa.beginPacket();
        LoRa.print(" ");
        LoRa.print(inputByte);
        LoRa.print(" ");
        LoRa.endPacket();
        Serial.print("SENT!  ");
        Serial.print(inputByte);
        Serial.print("\n");
        delay(200);
        LoRa.receive();
    }
}

void Socket::onReceive(auto handler) {
    LoRa.onReceive([=](int packetSize) {
        String message = "";
        while (LoRa.available()) {
            message += (char)LoRa.read();
        }
        if(message.length() == 2){
            int pos = this->udp_sendingStack.Find(message.toInt());
            if(pos != -1){
                this->udp_sendingStack.Erase(pos);
            }
        }
        handler(message);
    });
}

void Socket::udp(const String& msg, auto callback) {
    int mid = random(1000);
    String s = String(mid) + msg;
    this->udp_sendingStack.PushBack(mid);
    this->LoRa_sendMessage(s);
    for(int i = 1; i < SEND_TIMES; i ++){
        setTimeout([=](){
            this->LoRa_sendMessage(s);
            if(i == SEND_TIMES - 1){
                callback(0);
            }
        }, i*SEND_INTERVAL);
    }
}



void Socket::LoRa_rxMode(){
  LoRa.receive();
}

void Socket::LoRa_txMode(){
  LoRa.idle();
}


void Socket::LoRa_sendMessage(const String& message) {
  this->LoRa_txMode();
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  this->LoRa_rxMode();
}


#endif  //__SOCKET_HPP_