/*
 Name:		DCC_Funktionsdecoder.ino
 Created:	14.09.2020 10:22:04
 Author:	andy

 Bemerkungen:

siehe readme.md Programmieren des prozessors

*/

#include "DecoderCoreConfig.h"
#include "PinConfig.h"

// This is the version
#define DECODER_VERSION 4

// Default DCC Address
#define DEFAULT_DECODER_ADDRESS 3

#include "ATtiny_DecoderCore.h"

void setup() {

#ifdef LED_TEST 
    pinMode(LED_PIN_FWD, OUTPUT);
    pinMode(LED_PIN_REV, OUTPUT);
    pinMode(LED_PIN_AUX3, OUTPUT);
    pinMode(LED_PIN_AUX4, OUTPUT);


    digitalWrite(LED_PIN_FWD, HIGH);
    delay(1000);
    digitalWrite(LED_PIN_REV, HIGH);
    delay(1000);
    digitalWrite(LED_PIN_AUX3, HIGH);
    delay(500);
    digitalWrite(LED_PIN_AUX4, HIGH);
    delay(500);
    digitalWrite(LED_PIN_FWD, LOW);
    delay(500);
    digitalWrite(LED_PIN_REV, LOW);
    delay(500);
    digitalWrite(LED_PIN_AUX3, LOW);
    delay(500);
    digitalWrite(LED_PIN_AUX4, LOW);
    delay(500);
#endif // LED_TEST 
    setupDCC();
}

void loop() {
	loopDCC();
}
