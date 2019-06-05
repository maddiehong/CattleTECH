// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
 
#include <SPI.h>
#include <RH_RF95.h>
#include"pitches.h"
 
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

//Collar Mode setting
bool mode=0;

//Power setting, is awake=1, is asleep =0
bool isAwake;

//Average readings, by incrementing sum
int counter = 0;
float average = 0;
float sum = 0;

//Determine failures
int failureCounter = 0;

//Delay time between transmit
int waitTime=1000;
   
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  delay(100);
 
  Serial.println("Feather LoRa RX, Collar turning on!");
 
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
  
  //Blink LED on at init
  digitalWrite(LED, HIGH);
  delay(10);
  digitalWrite(LED, LOW);
  delay(10);
 
  // init Freq
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  //turn on to full power mode
  isAwake = 1;
}
 
void loop()
{
  delay(waitTime);
  if (rf95.available())
  {
    //reset failurenumbers
    failureCounter = 0;
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
      
      //increment sum and counter
      sum += rf95.lastRssi();
      counter++;
          
      //after 10 readings, check average
      if(counter==10)
      {
        //take average of last 10 RSSI readings
       average = (sum/10);
        
        Serial.println("Reading found");
        Serial.print("RSSI: ");
        Serial.println(average, DEC);
        Serial.println(sum, DEC);
       //determine mode setting
       modeSetter(buf);

       //based on mode, if in sound range, play sound
       if (average < -50)
       {
        playTone();
        digitalWrite(LED, LOW);
       }
       /* based on mode, if in shock range, send shock
       
       * insert shocker code here*/
       //based on mode, if in safe range, do nothing
       if (average >=-50)
       {
        digitalWrite(LED, HIGH);
       }

       //reset sum and counter
       sum = 0;
       counter = 0;
      }
    }
  }
  
  //after 10 min, determine out of range
  if (failureCounter == 600){
    Serial.println("Receive failed:out of range");
    failureCounter = 0;
  }
  else
    {
      waitTime = 1000;
      failureCounter++;
      Serial.print("Waiting... ");
      Serial.print(failureCounter);
      Serial.println(" packets");
    }
}

void playTone(){
  Serial.println("Collar in sound range");
  Serial.println("PLAYING WARNING SOUND");
  float exNote = 500;
  int speed = 2;
  for (float thisNote = 0; thisNote <3; thisNote+=0.01) {
    int noteDuration = speed*4;
    tone(6, exNote, noteDuration*.95);
    delay(noteDuration);
    exNote += thisNote;
    noTone(6);
  }
}

/*check packet data for repel or attract mode*/
bool modeSetter(uint8_t[]){
}

/*checks if cow is moving based on accelerometer readings
 * if moving, will change isAwake from asleep (0) to awake (1)
 */
bool MovementChecker(){
}

 
