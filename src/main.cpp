#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include <string.h>
#include "ubcRocketLogo.h"
#include <Wire.h>
#include <EEPROM.h>

//Definition of constants
#define LED_PIN PC13

//these pins will change
#define TFT_CS PA8
#define TFT_RST PA9
#define TFT_A0 PA10

#define ARMING_KEY PB9
#define E_MATCH PC15

//Definition of contant variables
const bool sender = false;

const long frequency = 433E6;

const int csPin = PA4;				 // LoRa radio chip select
const int resetPin = PA3;		 // LoRa radio reset
const int irqPin = PB11;				// change for your board; must be a hardware interrupt pin

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
void ISR_pb11();

bool interrupt_flag = false;

void setup() {

  //Pin initialization
  pinMode(LED_BUILTIN, OUTPUT);

  //serial monitor for debugging. CHECK BAUD RATE
  if(sender) {
    tft.initR(INITR_BLACKTAB);

    tft.fillScreen(ST7735_WHITE);
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setRotation(1);

    // tft.fillScreen(ST7735_WHITE);
    tft.drawRGBBitmap(35, 10, ubcRocket, UBCROCKET_WIDTH, UBCROCKET_HEIGHT);
    
    delay(2500);

    tft.fillScreen(ST7735_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 10);
    tft.setTextWrap(true);
    char msg[] = "Lorem ipsum dolor sijhjfgfdt amet, consectetur adipighfds elit, sed do eiusmod tempor incididunt ut labore et dolore";
    printText(10, tft, msg);

    delay(2500);
  } else {
    delay(5000);
  }
 
  LoRa.setPins(csPin, resetPin, irqPin);

  pinMode(LED_BUILTIN, OUTPUT);
	while (!LoRa.begin(frequency)) {
		digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);	
    delay(500);									
	}

  if(!sender) {
    pinMode(E_MATCH, OUTPUT);
  } 

  //interupt routines
  attachInterrupt(ARMING_KEY, ISR_pb11, HIGH);
}

bool send_state = false;
bool arming_signal_received = false;
bool send_LoRa(char string[]);

void loop() {

  if(!sender) {
    int packet_size = LoRa.parsePacket();

    if(packet_size) {
      tft.fillScreen(ST7735_WHITE);
      tft.setCursor(20, 20);
      tft.print("signal received!");
    } else {
      tft.fillScreen(ST7735_WHITE);
      tft.drawRGBBitmap(35, 10, ubcRocket, UBCROCKET_WIDTH, UBCROCKET_HEIGHT);
    }
    delay(1000);

    //get arming on the receiver end - look for interrupt flag

    //send to the sender 

    //then look for receiving signal from the sender. 

    //if input signal is received, after a little bit of delay, 

     if (interrupt_flag) {
      if(send_LoRa("armed")) {
        int packet_size = LoRa.parsePacket();

        if(packet_size) {
          //anlayze and then send e-match signal. 
        }
      }
    }
  }

  else if(sender) {
    int packet_size = LoRa.parsePacket();

    if(packet_size) {
      //read packet

      arming_signal_received = true;
    }

    if (arming_signal_received) {

      delay(500);

      if (interrupt_flag) {
        LoRa.beginPacket();
        LoRa.print("send");
        LoRa.endPacket();

        delay(100);
      }
      else if(!interrupt_flag) {
        //display basic screen instructions for arming and sending
      }
    }
    else {
      //display something
    }
  }
}

void reset() {
  send_state = false;
}

void ISR_pb11() {
  if(sender && arming_signal_received) {
    interrupt_flag = true;
  }
  else if(!sender) {
    interrupt_flag = true;
  }
}

bool send_LoRa(char string[]) {
  int count = 0;
  while(count < 10) {
    LoRa.beginPacket();
    LoRa.print(string);
    LoRa.endPacket();

    delay(100);

    count++;
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

//printing text. Can use for error messages
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

    if(word_iterator*6 > display.height() - padding) {
        for(int j = 0; j < word_iterator; j++) {
        display.print(word[j]);
        }
        padding_vertical += next_line_padding;
        display.setCursor(padding, padding_vertical);
        word_iterator = 0;
    }

    word[word_iterator] = msg[i];
    word_iterator++;
  }
}