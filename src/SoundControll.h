// SoundControll.h

#ifndef _SOUNDCONTROLL_h
#define _SOUNDCONTROLL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>


uint8_t lastVolume = 0;
uint8_t newVolume = 0;

uint8_t lastSound = 0;
uint8_t newSound = 0;

void SoundSetup();

void SoundLoop();

SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#endif


