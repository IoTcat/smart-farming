# 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
# 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
# 2 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino" 2



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
  //setInterval([](){
  //  socket.udp("hello socket!!!"+String(random(2000)), [](int e){
  //    Serial.println("send ok"+String(random(2000)));
  //  });
  //}, 7000);

}
