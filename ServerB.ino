#include <Ethernet.h>
#include <Arduino.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <EthernetUdp.h>
#include "TimerOne.h"

byte mac[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

#define PORT 30855
#define MAX_BUFFER 10

unsigned int port = PORT;
ZsutIPAddress client_ip = ZsutIPAddress(192.168.89.10);
ZsutEthernetUDP Udp;

unsigned int timeout = 4750; //ms

void setup() {
  Serial.begin(115200);
  Serial.println(F("ServerB initializing..."));
  
  ZsutEthernet.begin(mac);
  Serial.print(F("ServerB initialized, IP="));
  Serial.println(ZsutEthernet.localIP());

  ZsutPinMode (ZSUT_PIN_D3, OUTPUT);

  Serial.print(F("Started listening on port "));
  Serial.println(port);


  Serial.print(F("Sending HELLO message '2' to client on IP "));
  Serial.println(client_ip);
  Udp.begin(port);
  Udp.beginPacket(client_ip, port);
  int resp = Udp.write('2');
  Udp.endPacket();
  Udp.stop();
}

void loop() {
  Udp.begin(port);
  int packetSize = Udp.parsePacket();
  uint_16 read = ZsutAnalog1Read();
  Udp.begin(port);
  int packetSize = Udp.parsePacket();
  if(packetSize != 0){
      int result = Udp.read(packetBuffer, MAX_BUFFER);
      Serial.print(F("Reading humidity level"))
      if(read>=471){
          Serial.print(F("Reading very high humidity"))
          Serial.println(F("Sending message to client"));
          Udp.beginPacket(clientIP, port);
          int r=Udp.write("HIGH");
          Udp.endPacket();
       else if(read < 471){
          Udp.beginPacket(clientIP, port);
          int r=Udp.write("LOW");
          Udp.endPacket();
       }
          else if(strcmp(packetBuffer, "CHECK")){
          Serial.println(F("Sending answer to CHECK message"));
          Udp.beginPacket(clientIP, port);
          int r=Udp.write("SBR");
          Udp.endPacket();
          Serial.println(F("Sent answer to client: "));
          Serial.println(clientIP);
        }
          
      }
  }

  
  if(packetSize != 0){
     char response_buf;
     Serial.print(F("Got packet from client with message: "));
     int resp = Udp.read(&response_buf, sizeof(response_buf));
     Serial.println(response_buf);
     
     if(resp=='')
          shineDiode();

     }
     Udp.stop();
     delay(100);

    
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
          int r=Udp.write("S1R");
          Udp.endPacket();
          Serial.println(F("Sent answer to client: "));
          Serial.println(clientIP);
        }
    }
}
