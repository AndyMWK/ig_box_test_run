#include <Arduino.h>
#include "st7735.h"
#include "st7735_reg.h"
#include "LoRa.h"
#include <SPI.h>

//Definition of constants
#define LED_PIN PA11
#define LoRa_ID 915E6

//Definition of contant variables
const int tries_before_quit = 10;
const bool transmitter = true;

//create display object
ST7735_Object_t display;

//function declarations
void reset();

// set up function: 
// - set up pins
// - intialize LoRa module
// - initalize display
// - MUST DO ANY KIND OF ERROR HANDLING SHOULD THERE BE AN ERROR WITH SETUP 
//   (USE "tries_before_quit" to handle error. Program should attempt to initialize 
//   the object by this amount)

void setup() {

  //Pin initialization
  pinMode(LED_PIN, OUTPUT);

  //serial monitor for debugging. CHECK BAUD RATE
  Serial.begin(9600);
  
  //LoRa initalization
  if(!LoRa.begin(LoRa_ID)) {
    Serial.println("LoRa initialization failed!");
    int iterations = 0;

    while(iterations < tries_before_quit) {
      if(LoRa.begin(LoRa_ID)) {
        iterations = tries_before_quit;
      } else {
        iterations++;
      }
    }
  }

  //Display initialization
  if(ST7735_Init(&display, ST7735_FORMAT_RBG565, ST7735_ORIENTATION_LANDSCAPE) == ST7735_ERROR) {
    Serial.println("Display initialization failed!");
    int i = 0;
    while(i < tries_before_quit) {
      if(!ST7735_Init(&display, ST7735_FORMAT_RBG565, ST7735_ORIENTATION_LANDSCAPE) == ST7735_ERROR) {
        i = tries_before_quit;
      } else {
        i++;
      }
    }
  }
  //validation state - validates or timeout
}

bool send_state = false;
void loop() {
  //use state machines
  
  //ground state - looks for armed state activation

  //armed state - lasts for as long as it needs to

  //send state - send signal and transition to ground state
  if(send_state) {
    LoRa.beginPacket();
    LoRa.print(1);
    LoRa.endPacket();
    reset();
  }
  

  
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}

void reset() {
  send_state = false;
}
