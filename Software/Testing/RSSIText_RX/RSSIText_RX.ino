// Feather9x_RX
//based on Adafruit example code
//Test RSSI readings on simple data packet message
 
#include <SPI.h>
#include <RH_RF95.h>
 
// for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 433.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 13

int counter = 0;
int failureCounter = 0;
   
void setup()
{
  failureCounter = 0;
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  delay(100);
 
  Serial.println("Feather LoRa RX Repel Test!");
 
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
  //turn LED on at init
  digitalWrite(LED, HIGH);
  delay(1000);
 
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
  rf95.setTxPower(23, false);
  Serial.println("Starting Test");
}
 
void loop()
{
  delay(1000);
  if (rf95.available())
  {
    // Should be a message for us now
    failureCounter = 0;
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
        Serial.print("Got: ");
        Serial.println((char*)buf);
        Serial.print("Transmission #: ");
        Serial.println(counter);
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);
        Serial.print("Timer: ");
        Serial.println(millis());
        counter++;
    }
  }
  //after 1 min, determine out of range
  if (failureCounter == 60){
    Serial.println("Receive failed:out of range");
    failureCounter = 0;
  }
    else
    {
      failureCounter++;
      Serial.print("Waiting... ");
      Serial.print(failureCounter);
      Serial.println(" seconds");
    }
}
