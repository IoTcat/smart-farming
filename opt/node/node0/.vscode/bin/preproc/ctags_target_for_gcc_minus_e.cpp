# 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"
# 1 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino"


# 4 "e:\\git\\smart-farming\\opt\\node\\node0\\node0.ino" 2

LoRaMQTT mqtt;

void(* resetFunc) (void) = 0;

void setup() {

    Serial.begin(115200);

    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

    pinMode(A0, 0x0);
    pinMode(A1, 0x0);
    pinMode(A2, 0x0);
    pinMode(A3, 0x0);
    pinMode(4, 0x1);

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
  s += (float)analogRead(A0)/10;
  s += ",\"s\":";
  s += 0;
  s += ",\"q\":";
  s += content;
  s += "}";

}





bool pumpOn(){
  digitalWrite(4, 0x1);
  Serial.println("pumpOn");
  return true;
}

bool pumpOff(){
  digitalWrite(4, 0x0);
  Serial.println("pumpOff");
  return true;
}
