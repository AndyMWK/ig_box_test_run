#include <Arduino.h>
#include "LoRa.h"
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include <string.h>

//Definition of constants
#define LED_PIN PC13
#define LoRa_ID 915E6

//these pins will change
#define TFT_CS PA8
#define TFT_RST PA9
#define TFT_A0 PA0

//Definition of contant variables
const int tries_before_quit = 10;
const bool transmitter = true;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_A0, TFT_RST);

//create display object

//function declarations
void reset();

// set up function: 
// - set up pins
// - intialize LoRa module
// - initalize display
// - MUST DO ANY KIND OF ERROR HANDLING SHOULD THERE BE AN ERROR WITH SETUP 
//   (USE "tries_before_quit" to handle error. Program should attempt to initialize 
//   the object by this amount)

void printText(uint16_t padding, Adafruit_ST7735 &display, char msg[]);

void setup() {

  //Pin initialization
  pinMode(LED_BUILTIN, OUTPUT);

  //serial monitor for debugging. CHECK BAUD RATE
  tft.initR(INITR_BLACKTAB);

  tft.fillScreen(ST7735_WHITE);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setRotation(1);
  // tft.setCursor(10, 10);
  tft.setTextWrap(true);
  // tft.print("Hey, STM32 BLUE PILL. Lorum Ipsum asdfas lkkdfiea LDFdifa");
  char msg[] = "Lorem ipsum dolor sijhjfgfdt amet, consectetur adipighfds elit, sed do eiusmod tempor incididunt ut labore et dolore";

  printText(10, tft, msg);
  
  //LoRa initalization
  // if(!LoRa.begin(LoRa_ID)) {
  //   Serial.println("LoRa initialization failed!");
  //   int iterations = 0;

  //   while(iterations < tries_before_quit) {
  //     if(LoRa.begin(LoRa_ID)) {
  //       iterations = tries_before_quit;
  //     } else {
  //       iterations++;
  //     }
  //   }
  // }


  //validation state - validates or timeout
}

bool send_state = false;

void loop() {
  //use state machines
  
  //ground state - looks for armed state activation

  //armed state - lasts for as long as it needs to

  //send state - send signal and transition to ground state

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  
  // if(send_state) {
  //   LoRa.beginPacket();
  //   LoRa.print(1);
  //   LoRa.endPacket();
  //   reset();
  // }
}

void reset() {
  send_state = false;
}

void printText(uint16_t padding, Adafruit_ST7735 &display, char msg[]) {

  display.setCursor(padding, padding);

  
  //one way to iterate through a string
  size_t size_msg = strlen(msg);
  int size = (int)size_msg;

  int next_line_padding = 15;
  int padding_vertical = padding;
  int word_iterator  = 0;

  char word[display.height()];
  for(int i = 0; i < size; ++i) {

    // if(msg[i] == ' ' || msg[i] == '\n') {

      if(word_iterator*6 > display.height() - padding) {
        for(int j = 0; j < word_iterator; j++) {
        display.print(word[j]);
        }
        padding_vertical += next_line_padding;
        display.setCursor(padding, padding_vertical);
        word_iterator = 0;
      }
    // }
    word[word_iterator] = msg[i];
    word_iterator++;
  }
}