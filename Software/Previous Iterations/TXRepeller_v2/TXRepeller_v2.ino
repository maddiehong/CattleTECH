// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch for repeller post
 
#include <SPI.h>
#include <RH_RF95.h>

 
// Defining libraries for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
 
// Set testing Freq to 433.0 
#define RF95_FREQ 433.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

//setup PINS
#define onLED 6
#define repelLED 7
#define containLED 8
#define onButton 3
#define modeButton 4

//Mode setting, 0 = repel, 1 = contain
bool mode;

void setup() 
{
//setup PINS
  pinMode(onLED, OUTPUT);
  pinMode(repelLED, OUTPUT);
  pinMode(containLED, OUTPUT);
  pinMode(onButton, INPUT);
  pinMode(modeButton, INPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
 
  delay(100);
 
  Serial.println("Feather LoRa TX Repel Test!");
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // you can set transmitter powers from 5 to 23 dBm:  
  //set to lower power for higher resolution
  rf95.setTxPower(23, false);
}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission
 
void loop()
{
  // read the state of the modeButton value:
  int buttonState = digitalRead(modeButton);

  // check if the modeButton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn to contain mode, turn on LED
    digitalWrite(repelLED, LOW);
    digitalWrite(containLED, HIGH);
    mode = 1;
  } else {
    // turn to repel mode, turn on LED
    digitalWrite(containLED, LOW);
    digitalWrite(repelLED, HIGH);
    mode = 0;
  }
  
 //contain mode packet
  if (mode){
    
    delay(60000); // Wait 1 minute between transmits, could also 'sleep' here!
    Serial.println("Transmitting..."); // Send a message to rf95_server
    
    char radiopacket[20] = "R Packet # ";
    itoa(packetnum++, radiopacket+13, 10);
    Serial.print("Sending "); Serial.println(radiopacket);
    radiopacket[19] = 0;
    
    Serial.println("Sending...");
    delay(10);
    rf95.send((uint8_t *)radiopacket, 20);
   
    Serial.println("Waiting for packet to complete..."); 
    delay(10);
    rf95.waitPacketSent();
  }
  
 //repel mode packet
  else{
    delay(60000); // Wait 1 minute between transmits, could also 'sleep' here!
    Serial.println("Transmitting..."); // Send a message to rf95_server
    
    char radiopacket[20] = "C Packet # ";
    itoa(packetnum++, radiopacket+13, 10);
    Serial.print("Sending "); Serial.println(radiopacket);
    radiopacket[19] = 0;
    
    Serial.println("Sending...");
    delay(10);
    rf95.send((uint8_t *)radiopacket, 20);
   
    Serial.println("Waiting for packet to complete..."); 
    delay(10);
    rf95.waitPacketSent();
  }
}
