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
#define SEND_TIMES 8
#endif
#ifndef SEND_INTERVAL
#define SEND_INTERVAL 100
#endif
#ifndef TCP_TIMEOUT
#define TCP_TIMEOUT 1000
#endif


#define DEFAULT_SERIAL 115200


class Socket {
   public:
    Socket(){}
    void core();
    void ini();
    void onReceive(auto handler);
    void udp(const String& msg, auto callback);
    inline void udp(const String& msg) {
        this->udp(msg, [=](int err) {});
    };

   private:
    static Vector<int> udp_sendingStack;
    static String cache;
    static void (*callback)(const String& msg);
    void LoRa_rxMode();
    void LoRa_txMode();
    void LoRa_sendMessage(const String& message);
    static void _received(int size);

};

Vector<int> Socket::udp_sendingStack;
String Socket::cache = "";
void (*Socket::callback)(const String& msg);

void Socket::ini() {

    if(!Serial) Serial.begin(DEFAULT_SERIAL);

    //LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while(true);
    }
    LoRa.onReceive(this->_received);
}

void Socket::core() {

}

void Socket::_received(int size) {
    String message = "";
    while (LoRa.available()) {
        message += (char)LoRa.read();
    }
    Serial.println("  "+message);
    if (message == cache) return;
    cache = message;
    if (message.length() == 3) {
        int pos = udp_sendingStack.Find(message.toInt());
        if (pos != -1) {
            udp_sendingStack.Erase(pos);
        }
        return;
    }
    (*callback)(message.substring(3));
}

void Socket::onReceive(auto handler) {
     static auto f = handler;
     this->callback = f;
}

void Socket::udp(const String& msg, auto callback) {
    int mid = random(100, 1000);
    String s = String(mid) + msg;
    this->udp_sendingStack.PushBack(mid);
    this->LoRa_txMode();
    for(int i = 0; i < SEND_TIMES; i ++){
        setTimeout([=](){
            this->LoRa_sendMessage(s);
            if(i == SEND_TIMES - 1){
                this->LoRa_rxMode();
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
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}


#endif  //__SOCKET_HPP_