#include "socket.hpp"



Socket socket;

void callback(const String& s){
  Serial.println(s);
}

void setup(){

  Serial.begin(115200);
  socket.ini();
  socket.onReceive(callback);
}


void loop(){
  socket.core();
  setInterval([](){
    socket.udp("hello socket!!!"+String(random(2000)), [](int e){
      Serial.println("send ok"+String(random(2000)));
    });
  }, 7000);

}