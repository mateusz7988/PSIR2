#include <Ethernet.h>z
#include <EthernetUdp.h>
#include "TimerOne.h"
#define PORT 2936
#define MAX_BUFFER 10
unsigned int port = PORT;
byte mac[] = {0xAA,0xBB,0xCC,0xDE,0x1f};
IPAddress clientIP(10,6,84,102);
EthernetUDP Udp;
unsigned char packetBuffer[MAX_BUFFER];
void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(9600);
  
  Ethernet.begin(mac);
  Serial.println(Ethernet.localIP());
  Timer1.initialize(1000);
  Udp.begin(port);

  Serial.println(F("Sending HELLO to client on IP: "));
  Serial.println(clientIP);
  Udp.beginPacket(clientIP, port);
  int r=Udp.write("SA");
  Udp.endPacket();
  Udp.stop();
  Serial.println(F("Sent HELLO"));
  
}

void loop() {
  Udp.begin(port);
  int packetSize = Udp.parsePacket();
  if(packetSize != 0){
     int result = Udp.read(packetBuffer, MAX_BUFFER);
     if(strcmp(packetBuffer, "START")){
      Serial.println(F("Buzzing started"));
      Timer1.attachInterrupt(buzzBuzzer);
      }
      else if(strcmp(packetBuffer, "STOP")){
       Serial.println(F("Buzzing stopped"));
       stopBuzzer();
      }
      else if(strcmp(packetBuffer, "CHECK")){
          Serial.println(F("Sending answer to CHECK message"));
          Udp.beginPacket(clientIP, port);
          int r=Udp.write("SAR");
          Udp.endPacket();
          Serial.println(F("Sent answer to client: "));
          Serial.println(clientIP);
        }
    }
}

void buzzBuzzer(){
  digitalWrite(5, digitalRead(5)^1);
  }

void stopBuzzer(){
  digitalWrite(5, 0);
  }
