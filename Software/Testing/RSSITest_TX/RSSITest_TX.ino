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

long previousMillis = 0; //init previous timer counter
int interval = 2000; // set interval to every 5 seconds
 
void setup() 
{
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
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(5, false);
  pinMode(13, OUTPUT);
}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission
 
void loop()
{
  //Wait 10 seconds between transmits
  unsigned long currentMillis = millis();
  digitalWrite(13, HIGH);
  
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    Serial.println("Transmitting..."); // Send a message to rf95_server
    
    char radiopacket[20] = "Hello World #      ";
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
