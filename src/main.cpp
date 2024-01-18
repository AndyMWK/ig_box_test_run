#include <Arduino.h>
#include "LoRa.h"
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

//Definition of constants
#define LED_PIN PC13
#define LoRa_ID 915E6

//these pins will change
#define TFT_CS A8
#define TFT_RST A9
#define TFT_A0 A1

//Definition of contant variables
const int tries_before_quit = 10;
const bool transmitter = true;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_A0, TFT_RST);

//create display object

//function declarations
void reset();
void tftPrintTest();

// set up function: 
// - set up pins
// - intialize LoRa module
// - initalize display
// - MUST DO ANY KIND OF ERROR HANDLING SHOULD THERE BE AN ERROR WITH SETUP 
//   (USE "tries_before_quit" to handle error. Program should attempt to initialize 
//   the object by this amount)

void setup() {

  //Pin initialization
  pinMode(LED_BUILTIN, OUTPUT);

  //serial monitor for debugging. CHECK BAUD RATE
  tft.initR(INITR_BLACKTAB);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Hello, STM32 Blue Pill!");
  
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

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.print(6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST7735_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST7735_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST7735_WHITE);
  tft.print(" seconds.");
}