
#include "CV.h"

// Default CV Values Table
CVPair FactoryDefaultCVs[] = {
    // The CV Below defines the Short DCC Address
    {CV_MULTIFUNCTION_PRIMARY_ADDRESS, DEFAULT_DECODER_ADDRESS},

    // Three Step Speed Table
    {CV_VSTART, 120},
    {CV_VHIGH, 255},

    { CV_VERSION_ID, DECODER_VERSION },		// Version
    { CV_MANUFACTURER_ID,	13 },			// �ffentliche und Selbstbaudekoder

    { CV_DECODER_UNLOCK ,	0 },
    { CV_DECODER_CARRIER ,	0 },

    {CV_ANALOG_MODE, 0},                     // Aux1 = 1, Aux2 = 2, Aux3 = 4, Aux4 = 8, Aux5 = 16

    // These two CVs define the Long DCC Address
    {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},
    {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, DEFAULT_DECODER_ADDRESS},

    // ONLY uncomment 1 CV_29_CONFIG line below as approprate
    //  {CV_29_CONFIG,                                      0}, // Short Address 14 Speed Steps
    {CV_29_CONFIG,                       CV29_F0_LOCATION}, // Short Address 28/128 Speed Steps
    //  {CV_29_CONFIG, CV29_EXT_ADDRESSING | CV29_F0_LOCATION}, // Long  Address 28/128 Speed Steps  


  // Funktionstastenzuordnung F0 - F28
  // Aux1 = 1, Aux2 = 2, Aux3 = 4, Aux4 = 8, Aux5 = 16
    {CV_AUX_FN_ASSIGNMENT_BASE + 0, 3},  // F0
    {CV_AUX_FN_ASSIGNMENT_BASE + 1, 4},  // F1
    {CV_AUX_FN_ASSIGNMENT_BASE + 2, 8},  // F2
    {CV_AUX_FN_ASSIGNMENT_BASE + 3, 16},  // F3
    {CV_AUX_FN_ASSIGNMENT_BASE + 4, 0},  // F4
    {CV_AUX_FN_ASSIGNMENT_BASE + 5, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 6, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 7, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 8, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 9, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 10, 0},

    {CV_AUX_FN_ASSIGNMENT_BASE + 11, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 12, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 13, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 14, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 15, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 16, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 17, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 18, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 19, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 20, 0},

    {CV_AUX_FN_ASSIGNMENT_BASE + 21, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 22, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 23, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 24, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 25, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 26, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 27, 0},
    {CV_AUX_FN_ASSIGNMENT_BASE + 28, 0},

#ifdef fDecoder
    {CV_AUX1_DIR, DIR_BIT_FWD + 1},
    {CV_AUX2_DIR, DIR_BIT_REV + 1},
    {CV_AUX3_DIR, DIR_BIT_BOTH + 1},
#endif // fDecoder

#ifdef cDecoder
    {CV_AUX1_DIR, DIR_BIT_BOTH + 1},
    {CV_AUX2_DIR, DIR_BIT_BOTH + 1},
    {CV_AUX3_DIR, DIR_BIT_BOTH + 1},
#endif // cDecoder

#ifndef sDecoder
    {CV_AUX4_DIR, DIR_BIT_BOTH},
    {CV_AUX5_DIR, DIR_BIT_BOTH},
#endif // !sDecoder

    {CV_AUX1_MODE, 1},
    {CV_AUX2_MODE, 1},
    {CV_AUX3_MODE, 3},


#ifndef sDecoder
    {CV_AUX4_MODE, 2},
    {CV_AUX5_MODE, 2},
#endif // !sDecoder

    {CV_AUX1_TIME_ON, 100},
    {CV_AUX2_TIME_ON, 100},
    {CV_AUX3_TIME_ON, 100},

#ifndef sDecoder
    {CV_AUX4_TIME_ON, 100},
    {CV_AUX5_TIME_ON, 100},
#endif // !sDecoder

    {CV_AUX1_TIME_OFF, 100},
    {CV_AUX2_TIME_OFF, 100},
    {CV_AUX3_TIME_OFF, 100},

#ifndef sDecoder
    {CV_AUX4_TIME_OFF, 100},
    {CV_AUX5_TIME_OFF, 100},
#endif // !sDecoder

    {CV_AUX1_BRIGHTNESS, 255},
    {CV_AUX2_BRIGHTNESS, 255},

    {CV_AUX1_AUTO_OFF, 0},
    {CV_AUX2_AUTO_OFF, 0},
    {CV_AUX3_AUTO_OFF, 100},

#ifndef sDecoder
    {CV_AUX4_AUTO_OFF, 0},
    {CV_AUX5_AUTO_OFF, 0},
#endif // !sDecoder

    {CV_AUX_Save, 0},


#ifdef sDecoder
    {CV_SND_VOLUME, 10},
    {CV_SND_EQ,  DFPLAYER_EQ_NORMAL},

    // Soundzuordnung zu  F0 - F28
    {CV_SND_FN_ASSIGNMENT_BASE + 0, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 1, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 2, 8},
    {CV_SND_FN_ASSIGNMENT_BASE + 3, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 4, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 5, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 6, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 7, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 8, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 9, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 10, 250},  // Mute, unmute

    {CV_SND_FN_ASSIGNMENT_BASE + 11, 251},  // Volume up
    {CV_SND_FN_ASSIGNMENT_BASE + 12, 252},  // Volume down
    {CV_SND_FN_ASSIGNMENT_BASE + 13, 8},
    {CV_SND_FN_ASSIGNMENT_BASE + 14, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 15, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 16, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 17, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 18, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 19, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 20, 0},

    {CV_SND_FN_ASSIGNMENT_BASE + 21, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 22, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 23, 8},
    {CV_SND_FN_ASSIGNMENT_BASE + 24, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 25, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 26, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 27, 0},
    {CV_SND_FN_ASSIGNMENT_BASE + 28, 0},

#endif // sDecoder


    {CV_AUX_State, 0}

};
