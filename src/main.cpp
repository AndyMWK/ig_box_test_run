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
#define TFT_LED PB15

#define ARMING_KEY PB9
#define E_MATCH PB10
#define MESSAGE_SIZE_MAX 5


//GPIO Pins 
#define GPIO1 PB6 //output, always high for sender and receiver
#define SENDER_FINAL_INPUT PA1
#define GPIO2 PB3
#define GPIO3 PA1


//LED Blinking frequency to indicate error
#define LORA_INIT_ERROR 200
#define ARMED_LORA_SEND 500
#define ARMED_SIGNAL_NOT_RECEIVED 1000
#define MAX_ERROR_ATTEMPTS 10

//System Global Variables - dictate what mode the device is operating in. 
const bool sender = false;
void blink_LED_for_error(uint32_t time_ms);
bool arming_signal_received = false;

//LoRa Global Variables
const long frequency = 433E6;

const int csPin = PA4;				  // LoRa radio chip select
const int resetPin = PA3;		    // LoRa radio reset
const int irqPin = PB11;				// change for your board; must be a hardware interrupt pin

//Signal values to send
char igniter_arming_signal = '1';
char ignite_signal = '2';
char ignite_complete = '3';

//LoRa function declarations
bool read_packet(char correct_message);
bool send_LoRa(char message);

//TFT Screen Global Variables and Function Declarations
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_A0, TFT_RST);

void setup() {
  
  if(sender) {
    //initializing TFT LED Output
    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, HIGH);

    //initialize TFT screen
    tft.initR(INITR_BLACKTAB);

    tft.fillScreen(ST7735_WHITE);
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setRotation(1);

    // tft.fillScreen(ST7735_WHITE);
    tft.drawRGBBitmap(35, 10, ubcRocket, UBCROCKET_WIDTH, UBCROCKET_HEIGHT);
    
    delay(5000);

    pinMode(GPIO1, OUTPUT);
    pinMode(SENDER_FINAL_INPUT, INPUT);

    digitalWrite(GPIO1, HIGH);
    
  } else {

    //initalize all peripherals
    pinMode(GPIO1, OUTPUT);
    pinMode(GPIO2, OUTPUT);
    pinMode(GPIO3, INPUT);
    pinMode(E_MATCH, OUTPUT);

    digitalWrite(GPIO1, LOW);
    digitalWrite(GPIO2, HIGH);
    digitalWrite(E_MATCH, LOW);
    delay(5000);
  }
  
  //initalize LoRa Module
  LoRa.setPins(csPin, resetPin, irqPin);

  //for debugging. LED will blink if the LoRa module is not initialized. 
  pinMode(LED_BUILTIN, OUTPUT);

	while (!LoRa.begin(frequency)) {
		blink_LED_for_error(LORA_INIT_ERROR);							
	}
}

void loop() {

  //------------------------------igniter----------------------------
  if(!sender) {

    if(analogRead(GPIO3) >= 900) {
      
      //Turns the buzzer on
      //digitalWrite(GPIO1, HIGH);

      if(send_LoRa(igniter_arming_signal)) {

        //Lets the uesr know that signal is being sent to the other device
        blink_LED_for_error(100);

        //immidiately looks for the ignition signal coming from the other device
        if(read_packet(ignite_signal)) {

          //Turns the E-Match on when the ignition signal is received. 
          digitalWrite(E_MATCH, HIGH);
          delay(2000);
          digitalWrite(E_MATCH, LOW);
          
          //sends acknowledge signal to the other device then turns off buzzer
          if(send_LoRa(ignite_complete)) {
            digitalWrite(GPIO1, LOW);
          }
        }
      } else {
        blink_LED_for_error(ARMED_SIGNAL_NOT_RECEIVED);
      }
    } else {
      digitalWrite(GPIO1, LOW);
    }
  }

  //------------------------------sender----------------------------
  else if(sender) {

    //constantly looking for signal coming from the other device for ignition
    if(read_packet(igniter_arming_signal)) {
      arming_signal_received = true;
    } else {
      blink_LED_for_error(ARMED_SIGNAL_NOT_RECEIVED);
      //display something here
    }

   while (arming_signal_received) {

      //waits for the user input for final ignition
      if(analogRead(SENDER_FINAL_INPUT) >= 900) {

        //for testing
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);	
        delay(500);	

        //sends the final 'ignite' signal to the other device
        while(send_LoRa(ignite_signal)) {

          //also for testing
          tft.fillScreen(ST7735_WHITE);
          tft.setCursor(20, 20);
          tft.print("IGNITE");

          //when the 'acknowledged' signal comes from the other device, then exit loop
          if(read_packet(ignite_complete)) {
            arming_signal_received = false;
            break;
          }
        }
        tft.fillScreen(ST7735_WHITE);
        //display something to show that the ignition process has been completed. 
      } else {
        tft.fillScreen(ST7735_WHITE);
        tft.setCursor(20, 20);
        tft.print("waiting for ignite");
        delay(500);
    }
  }
  }
}

//-------------------------------------------------------------------------------------------------

bool send_LoRa(char message) {

  if(!LoRa.beginPacket()) {
    return false;
  }
  if(!LoRa.print(message)) {
    return false;
  }
  if(!LoRa.endPacket()) {
    return false;
  }

  return true;
}

bool read_packet(char correct_message) {
  int packet_size = LoRa.parsePacket();

  if(packet_size) {

    while(LoRa.available()) {
      
      if(LoRa.read() == correct_message) {
        return true;
      }
    }
  }

  return false;
  
}

void blink_LED_for_error(uint32_t time_ms) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time_ms);
  digitalWrite(LED_BUILTIN, LOW);
  delay(time_ms);
}