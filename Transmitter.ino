#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

byte msgCount = 0;
byte localAddress = 0xBB;
byte destination = 0xFF;
long lastSendTime = 0;
int interval = 50;

#define sw1 3
#define sw2 4
#define sw3 5
#define sw4 6

int Sensor1;
int Sensor2;
int Sensor3;
int Sensor4;

int Data1;
int Data2;
int Data3;
int Data4;

int load1;
int load2;
int load3;
int load4;

int DataOut1;

boolean bflag = false;
String outgoing;            //outgoing message

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String pinStatus = "0";

String Mymessage = "";

int i;
String incomingString;
String tempString;
String feedString;
boolean rState;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 28);
  // Display static text
  display.println("Loading . . .");
  display.display(); 

  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);

  while(!Serial);
  Serial.println("LoRa Sender");
  if(!LoRa.begin(433E6)){
    Serial.println("Starting LoRa failed!");
    while(1);
  }

  LoRa.setTxPower(20);
}

void loop() {
  if(digitalRead(sw1) == LOW){
    Data1 = 1;
  }
  else{
    Data1 = 0;
  }
  if(digitalRead(sw2) == LOW){
    Data2 = 1;
  }
  else{
    Data2 = 0;
  }

  if(digitalRead(sw3) == LOW){
    Data3 = 1;
  }
  else{
    Data3 = 0;
  }

  if(digitalRead(sw4) == LOW){
    Data4 = 1;
  }
  else{
    Data4 = 0;
  }

  String Data = "";
  Data = Data + Data1 + "," + Data2 + "," + Data3 + "," + Data4;

  LoRa.beginPacket();
  LoRa.print(Data);
  LoRa.endPacket();

  delay(10);
  
  // Receive feedback from receiver
  String feedback = receiveFeedback();

  // Display data and feedback
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(20,18);
  display.println("Status:" + Data);
  display.setCursor(15,35);
  display.print("Feedback:" + feedback);
  display.display();
}

String receiveFeedback() {
  String feedback = "";
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      feedback += (char)LoRa.read();
    }
  }
  return feedback;
}
