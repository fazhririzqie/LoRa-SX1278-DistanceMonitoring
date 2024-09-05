#include <SPI.h>
#include <LoRa.h>

int Data1, Data2, Data3, Data4;
int RLY1=3;
int RLY2=4;
int RLY3=5;
int RLY4=6;
// stores the relays status
int relay1Status = 0;
int relay2Status = 0;
int relay3Status = 0;
int relay4Status = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  pinMode(RLY1,OUTPUT);
  pinMode(RLY2,OUTPUT);
  pinMode(RLY3,OUTPUT);
  pinMode(RLY4,OUTPUT);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    String receivedData = "";

    // read packet
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    Serial.print(receivedData);

    // parse the received data
    parseData(receivedData);

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    // send feedback to transmitter
    sendFeedback();
  }

  if(Data1 == 1){
    digitalWrite(RLY1,HIGH);
    relay1Status = 1;
    Serial.println("Relay 1 is turned on");
  }
  if(Data1 == 0){
    digitalWrite(RLY1,LOW);
    relay1Status = 0;
    Serial.println("Relay 1 is turned off");
  }
  if(Data2 == 1){
    digitalWrite(RLY2,HIGH);
    relay2Status = 1;
    Serial.println("Relay 2 is turned on");
  }
  if(Data2 == 0){
    digitalWrite(RLY2,LOW);
    relay2Status = 0;
    Serial.println("Relay 2 is turned off");
  }
  if(Data3 == 1){
    digitalWrite(RLY3,HIGH);
    relay3Status = 1;
    Serial.println("Relay 3 is turned on");
  }
  if(Data3 == 0){
    digitalWrite(RLY3,LOW);
    relay3Status = 0;
    Serial.println("Relay 3 is turned off");
  }
  if(Data4 == 1){
    digitalWrite(RLY4,HIGH);
    relay4Status = 1;
    Serial.println("Relay 4 is turned on");
  }
  if(Data4 == 0){
    digitalWrite(RLY4,LOW);
    relay4Status = 0;
    Serial.println("Relay 4 is turned off");
  }
}

void parseData(String data) {
  int commaIndex1 = data.indexOf(',');
  int commaIndex2 = data.indexOf(',', commaIndex1 + 1);
  int commaIndex3 = data.indexOf(',', commaIndex2 + 1);

  if (commaIndex1 != -1 && commaIndex2 != -1 && commaIndex3 != -1) {
    Data1 = data.substring(0, commaIndex1).toInt();
    Data2 = data.substring(commaIndex1 + 1, commaIndex2).toInt();
    Data3 = data.substring(commaIndex2 + 1, commaIndex3).toInt();
    Data4 = data.substring(commaIndex3 + 1).toInt();

    // Print the separated data to verify
    Serial.print("Data1: ");
    Serial.print(Data1);
    Serial.print(", Data2: ");
    Serial.print(Data2);
    Serial.print(", Data3: ");
    Serial.print(Data3);
    Serial.print(", Data4: ");
    Serial.println(Data4);
  } else {
    Serial.println("Error: Data format incorrect");
  }
}

void sendFeedback() {
  String feedback = String(relay1Status) + "," + String(relay2Status) + "," + String(relay3Status) + "," + String(relay4Status);
  
  LoRa.beginPacket();
  LoRa.print(feedback);
  LoRa.endPacket();
}
