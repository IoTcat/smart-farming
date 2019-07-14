#include "socket.hpp"


Socket socket;

void setup(){

  Serial.begin(115200);
  socket.ini();

}


void loop(){
socket.core();

}