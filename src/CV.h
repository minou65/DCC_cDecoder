// CV.h

#ifndef _CV_h
#define _CV_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <NmraDcc.h>

//Structure for CV Values Table
struct CVPair{
	uint16_t  CV;
	uint8_t   Value;
};

extern CVPair FactoryDefaultCVs[]


// CV Addresses we will be using
#define CV_VSTART  2
#define CV_VHIGH   5
#define CV_Reset    8

#define CV_ANALOG_MODE 13

#define CV_DECODER_UNLOCK	15		// Decoder unlock, muss den selben Wert haben wie CV_DECODER_CARRIER
#define CV_DECODER_CARRIER	16

#define CV_AUX_FN_ASSIGNMENT_BASE 30


#define CV_AUX1_DIR 61
#define CV_AUX2_DIR 62
#define CV_AUX3_DIR 63

#ifndef sDecoder

#define CV_AUX4_DIR 64
#define CV_AUX5_DIR 65

#endif // !sDecoder

#define CV_AUX1_MODE 66
#define CV_AUX2_MODE 67
#define CV_AUX3_MODE 68

#ifndef sDecoder

#define CV_AUX4_MODE 69
#define CV_AUX5_MODE 70

#endif // !sDecoder

#define CV_AUX1_TIME_ON 71
#define CV_AUX2_TIME_ON 72
#define CV_AUX3_TIME_ON 73

#ifndef sDecoder

#define CV_AUX4_TIME_ON 74
#define CV_AUX5_TIME_ON 75

#endif // !sDecoder

#define CV_AUX1_TIME_OFF 76
#define CV_AUX2_TIME_OFF 77
#define CV_AUX3_TIME_OFF 78

#ifndef sDecoder

#define CV_AUX4_TIME_OFF 79
#define CV_AUX5_TIME_OFF 80

#endif // !sDecoder

#define CV_AUX1_BRIGHTNESS  81
#define CV_AUX2_BRIGHTNESS  82

#define CV_AUX1_AUTO_OFF 86
#define CV_AUX2_AUTO_OFF 87
#define CV_AUX3_AUTO_OFF 88

#ifndef sDecoder

#define CV_AUX4_AUTO_OFF 89
#define CV_AUX5_AUTO_OFF 90

#endif // !sDecoder

#define CV_AUX_Save     122
#define CV_AUX_State    255

#ifdef sDecoder

#define CV_SND_VOLUME   125
#define CV_SND_EQ       126
#define CV_SND_FN_ASSIGNMENT_BASE 130

#endif // sDecoder




#endif

