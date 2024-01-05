#include <Arduino.h>
#include "st7735.h"
#include "st7735_reg.h"
#include "LoRa.h"
#include <SPI.h>


#define LED_PIN PA11
#define LoRa_ID 915E6

const int tries_before_quit = 10;
const bool transmitter = true;

ST7735_Object_t display;

void reset();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  
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
