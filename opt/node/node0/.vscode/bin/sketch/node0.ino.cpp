#include <Arduino.h>
#line 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
#line 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
#include "socket.hpp"



Socket socket;

#line 7 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void callback(const String& s);
#line 11 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void setup();
#line 19 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void loop();
#line 7 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
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
  //setInterval([](){
  //  socket.udp("hello socket!!!"+String(random(2000)), [](int e){
  //    Serial.println("send ok"+String(random(2000)));
  //  });
  //}, 7000);

}
