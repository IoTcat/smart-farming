#include <Arduino.h>
#line 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
#line 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
#include "socket.hpp"


Socket socket;

#line 6 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void setup();
#line 14 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void loop();
#line 6 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
void setup(){

  Serial.begin(115200);
  socket.ini();

}


void loop(){
socket.core();

}
