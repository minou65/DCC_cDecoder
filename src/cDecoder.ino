/*
 Name:		DCC_Funktionsdecoder.ino
 Created:	14.09.2020 10:22:04
 Author:	andy

 Bemerkungen:

siehe readme.md Programmieren des prozessors

*/



// ONLY uncomment 1 line below as approprate
#define fDecoder;
//#define cDecoder;
//#define sDecoder;

// Uncomment to force CV Reset to Factory Defaults
//#define FactoryDefaults
//#define EEPROM_Clear

// Uncomment any of the lines below to enable debug messages for different parts of the code
//#define DEBUG_FUNCTIONS
//#define DEBUG_FUNC
//#define DEBUG_SPEED
//#define DEBUG_PWM
//#define DEBUG_DCC_ACK
//#define DEBUG_DCC_MSG
//#define DEBUG_PRINT

// #define LED_TEST

#ifdef sDecoder
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

uint8_t lastVolume = 0;
uint8_t newVolume = 0;

uint8_t lastSound = 0;
uint8_t newSound = 0;

#endif

#include "LEDControll.h"
#include <EEPROM.h>
#include <NmraDcc.h>


#if defined(DEBUG_FUNCTIONS) or defined(DEBUG_SPEED) or defined(DEBUG_PWM) or defined(DEBUG_DCC_ACK) or defined(DEBUG_DCC_MSG) or defined(DEBUG_FUNC)
#define DEBUG_PRINT
#endif

// This is the default DCC Address
#define DEFAULT_DECODER_ADDRESS 3

// This is the version
#define DECODER_VERSION 4


#ifdef __AVR_ATmega328P__
// This section defines the Arduino UNO Pins to use 

#define DCC_PIN     2
#define DCC_PIN_INT 1   // Interrupt number of the pin

#define DCC_ACK_PIN 3

#define LED_PIN_FWD 3
#define LED_PIN_REV 5
#define LED_PIN_AUX3 6
#define LED_PIN_AUX4 9
#define LED_PIN_AUX5 10

#define NUM_OUTPUTS 5

#ifdef sDecoder
#define RX_PIN 10
#define TX_PIN 11
#endif // sDecoder


#elif defined(__AVR_ATtiny85__)
// This section defines the Arduino ATTiny85 Pins to use 

// on a attiny debugger do not run 
#undef DEBUG_FUNCTIONS
#undef DEBUG_SPEED
#undef DEBUG_PWM
#undef DEBUG_DCC_ACK
#undef DEBUG_DCC_MSG
#undef DEBUG_PRINT

#define DCC_PIN         2 
#define DCC_PIN_INT     PCINT2   // Interrupt number of the pin

#define LED_PIN_FWD     0   // AUX 1
#define LED_PIN_REV     1   // AUX 2
#define LED_PIN_AUX3    4  // AUX 3

#define DCC_ACK_PIN     LED_PIN_FWD

#ifdef sDecoder

#define RX_PIN 3
#define TX_PIN 4

#define NUM_OUTPUTS 3

#else

#define LED_PIN_AUX4 3

#define NUM_OUTPUTS 4

#endif // sDecoder

#else
#error "Unsupported CPU, you need to add another configuration section for your CPU"
#endif 

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

/* x=target variable, y=mask */
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK_ALL(x,y) (((x) & (y)) == (y))   // warning: evaluates y twice
#define BITMASK_CHECK_ANY(x,y) ((x) & (y))

// Bitmask array
const uint8_t bitmask[] = { 1, 2, 4, 8, 16 };

// Some global state variables

uint8_t lastAUXState = 0;  // Save the State for the Outputs
bool saveAUXState = false;

uint8_t newDirection = 0;
uint8_t lastDirection = 0;

uint8_t newSpeed = 0;
uint8_t lastSpeed = 0;
uint8_t numSpeedSteps = SPEED_STEP_128;

uint8_t vStart;
uint8_t vHigh;
uint8_t analogMode;
uint8_t AllwaysOn;

#define AUX1 0
#define AUX2 1
#define AUX3 2
#define AUX4 3
#define AUX5 4

LED Aux[5];

#ifdef sDecoder
SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
#endif


//Structure for CV Values Table
struct CVPair
{
    uint16_t  CV;
    uint8_t   Value;
};



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

#define CV_ALLWAYS_ON   123

#ifdef sDecoder

#define CV_SND_VOLUME   125
#define CV_SND_EQ       126
#define CV_SND_FN_ASSIGNMENT_BASE 130

#endif // sDecoder



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
    {CV_ALLWAYS_ON, 0 },    // Aux1 = 1, Aux2 = 2, Aux3 = 4, Aux4 = 8, Aux5 = 16


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

NmraDcc  Dcc;

uint8_t FactoryDefaultCVIndex = 0;
unsigned long lastDCCTime = millis();        // when we last recived a DCC signal
unsigned long currentDCCInterval = 100;

// This call-back function is called when a CV Value changes so we can update CVs we're using
void notifyCVChange(uint16_t CV, uint8_t Value)
{
    switch (CV)
    {
        // case CV_MANUFACTURER_ID:
        break;

    case CV_VERSION_ID:
        break;

    case CV_DECODER_CARRIER:
        break;

    case CV_DECODER_UNLOCK:
        break;

    case CV_VSTART:
        vStart = Value;
        break;

    case CV_VHIGH:
        vHigh = Value;
        break;

#ifdef sDecoder
    case CV_SND_VOLUME:
        uint8_t volume = Dcc.getCV(CV_SND_VOLUME);
        if (volume > 30)
            volume = 30;
        myDFPlayer.volume(volume);  //Set volume value (0~30).
        break;

    case CV_SND_EQ:
        uint8_t eq = Dcc.getCV(CV_SND_EQ);
        if (eq > 5)
            eq = 5;
        myDFPlayer.EQ(eq);
        break;

#endif // sDecoder

    case CV_Reset:
        if (Value == 8) {
            notifyCVResetFactoryDefault();
        }
        break;

    case CV_ANALOG_MODE:
        analogMode = Dcc.getCV(CV_ANALOG_MODE);
        break;

    default:
        if (FactoryDefaultCVIndex == 0)
            initDecoder();
    }

}

uint8_t notifyCVRead(uint16_t CV) {
#ifdef DEBUG_FUNCTIONS
    Serial.print(F("notifyCVRead: CV: "));
    Serial.print(CV, DEC);
    Serial.print(F(" = "));
#endif

    uint16_t value = 0;


    switch (CV) {
    case CV_VERSION_ID:
        value = DECODER_VERSION;

    case CV_MANUFACTURER_ID:
        value = MAN_ID_DIY;

    default:
        value = eeprom_read_byte((uint8_t*)CV);
    }

#ifdef DEBUG_FUNCTIONS
    Serial.println(value, DEC);
#endif // DEBUG_FUNCTIONS

    return value;
}

uint8_t notifyCVWrite(uint16_t CV, uint8_t Value) {
#ifdef DEBUG_FUNCTIONS
    Serial.print(F("notifyCVWrite: CV: "));
    Serial.print(CV, DEC);
    Serial.print(F(" = "));
    Serial.println(Value, DEC);
#endif

    if ((Dcc.getCV(CV_DECODER_CARRIER) == Dcc.getCV(CV_DECODER_UNLOCK)) || CV == CV_DECODER_UNLOCK || CV == CV_AUX_State) {

#ifdef DEBUG_FUNCTIONS
        Serial.println(F("Decoder not looked or CV_DECODER_UNLOCK or CV_AUX_State"));
#endif

        if (eeprom_read_byte((uint8_t*)CV) != Value) {
            eeprom_write_byte((uint8_t*)CV, Value);

            if (notifyCVChange)
                notifyCVChange(CV, Value);
        }
        return eeprom_read_byte((uint8_t*)CV);

    };
    return 0;
};

void notifyCVResetFactoryDefault()
{
    // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
    // to flag to the loop() function that a reset to Factory Defaults needs to be done
    FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
};

// This call-back function is called whenever we receive a DCC Speed packet for our address 
void notifyDccSpeed(uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps)
{
#ifdef DEBUG_SPEED
    Serial.print("notifyDccSpeed: Addr: ");
    Serial.print(Addr, DEC);
    Serial.print((AddrType == DCC_ADDR_SHORT) ? "-S" : "-L");
    Serial.print(" Speed: ");
    Serial.print(Speed, DEC);
    Serial.print(" Steps: ");
    Serial.print(SpeedSteps, DEC);
    Serial.print(" Dir: ");
    Serial.println((Dir == DCC_DIR_FWD) ? "Forward" : "Reverse");
#endif

    newDirection = Dir;
    newSpeed = Speed;
    numSpeedSteps = SpeedSteps;

    for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
        Aux[i].newDirection(Dir);
    }

};

// This call-back function is called whenever we receive a DCC Function packet for our address 
void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState) {
#ifdef DEBUG_FUNC2
    Serial.print("notifyDccFunc: Addr: ");
    Serial.print(Addr, DEC);
    Serial.print((AddrType == DCC_ADDR_SHORT) ? "-S" : "-L");
    Serial.print("  Function Group: ");
    Serial.println(FuncGrp, DEC);
#endif

    switch (FuncGrp)
    {
#ifdef NMRA_DCC_ENABLE_14_SPEED_STEP_MODE    
    case FN_0:
        SetFuncState(0, (FuncState & FN_BIT_00) ? 1 : 0);
        break;
#endif

    case FN_0_4:


#ifdef DEBUG_FUNC
        uint8_t newLedState = (FuncState & FN_BIT_00) ? 1 : 0;
        Serial.print(" FN 0: ");
        Serial.println(newLedState);
#endif


        // Only process Function 0 in this packet if we're not in Speed Step 14 Mode
        if (Dcc.getCV(CV_29_CONFIG) & CV29_F0_LOCATION) {
            SetFuncState(0, (FuncState & FN_BIT_00) ? 1 : 0);
        }

        SetFuncState(1, (FuncState & FN_BIT_01) ? 1 : 0);
        SetFuncState(2, (FuncState & FN_BIT_02) ? 1 : 0);
        SetFuncState(3, (FuncState & FN_BIT_03) ? 1 : 0);
        SetFuncState(4, (FuncState & FN_BIT_04) ? 1 : 0);

        break;

    case FN_5_8:
        SetFuncState(5, (FuncState & FN_BIT_05) ? 1 : 0);
        SetFuncState(6, (FuncState & FN_BIT_06) ? 1 : 0);
        SetFuncState(7, (FuncState & FN_BIT_07) ? 1 : 0);
        SetFuncState(8, (FuncState & FN_BIT_08) ? 1 : 0);
        break;

    case FN_9_12:
        SetFuncState(9, (FuncState & FN_BIT_09) ? 1 : 0);
        SetFuncState(10, (FuncState & FN_BIT_10) ? 1 : 0);
        SetFuncState(11, (FuncState & FN_BIT_11) ? 1 : 0);
        SetFuncState(12, (FuncState & FN_BIT_12) ? 1 : 0);
        break;

    case FN_13_20:
        SetFuncState(13, (FuncState & FN_BIT_13) ? 1 : 0);
        SetFuncState(14, (FuncState & FN_BIT_14) ? 1 : 0);
        SetFuncState(15, (FuncState & FN_BIT_15) ? 1 : 0);
        SetFuncState(16, (FuncState & FN_BIT_16) ? 1 : 0);
        SetFuncState(17, (FuncState & FN_BIT_17) ? 1 : 0);
        SetFuncState(18, (FuncState & FN_BIT_18) ? 1 : 0);
        SetFuncState(19, (FuncState & FN_BIT_19) ? 1 : 0);
        SetFuncState(20, (FuncState & FN_BIT_20) ? 1 : 0);
        break;

    case FN_21_28:
        SetFuncState(21, (FuncState & FN_BIT_21) ? 1 : 0);
        SetFuncState(22, (FuncState & FN_BIT_22) ? 1 : 0);
        SetFuncState(23, (FuncState & FN_BIT_23) ? 1 : 0);
        SetFuncState(24, (FuncState & FN_BIT_24) ? 1 : 0);
        SetFuncState(25, (FuncState & FN_BIT_25) ? 1 : 0);
        SetFuncState(26, (FuncState & FN_BIT_26) ? 1 : 0);
        SetFuncState(27, (FuncState & FN_BIT_27) ? 1 : 0);
        SetFuncState(28, (FuncState & FN_BIT_28) ? 1 : 0);
        break;
    }

    // get AUX state 
    for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
        // (number & ~(1UL << n)) will clear the nth bit and (x << n) will set the nth bit to x.
        lastAUXState = (lastAUXState & ~(1UL << i)) | (int(Aux[i].state()) << i);

    }

#ifdef DEBUG_FUNC2
    Serial.print(F("AUX State: "));
    Serial.print(lastAUXState, BIN);

    Serial.println();
#endif

    if (saveAUXState)
        Dcc.setCV(CV_AUX_State, lastAUXState);

#ifdef DEBUG_FUNCTIONS
    Serial.println();
#endif
}

void SetFuncState(uint8_t Func, uint8_t state) {
#ifdef DEBUG_FUNCTIONS
    Serial.print("SetFuncState: Func: ");
    Serial.print(Func, DEC);
    Serial.print("  State: ");
    Serial.println(state, DEC);
#endif

    uint8_t value = Dcc.getCV(CV_AUX_FN_ASSIGNMENT_BASE + Func);

    for (uint8_t i = 0; i < 5; i++) {
        if (BIT_CHECK(value, i)) {
            Aux[i].newState(state);
        }

        if (BIT_CHECK(AllwaysOn, bitmask[i])) {
			Aux[i].newState(1);
		}

    }

#ifdef sDecoder


    uint8_t sound = Dcc.getCV(CV_SND_FN_ASSIGNMENT_BASE + Func);

    switch (sound) {
    case 0: // do nothing
        break;

    case 250:   // Mute, unmute
        if (state) {
            newVolume = 0;
        }
        else {
            newVolume = Dcc.getCV(CV_SND_VOLUME);
        }
        break;
    case 251:   // Volume up
        if ((lastVolume == newVolume) && (state)) {
            newVolume++;
            if (newVolume > 30)
                newVolume = 30;
        }
        break;
    case 252:   // Volume down
        if ((lastVolume == newVolume) && (state)) {
            newVolume--;
            if (newVolume < 1)
                newVolume = 1;
        }
        break;
    default: // Play sound
        if ((state) && (lastSound != value))
            newSound = value;

    }
#endif // sDecoder


}

// This call-back function is called whenever we receive a DCC Packet
void notifyDccMsg(DCC_MSG* Msg) {
#ifdef  DEBUG_DCC_MSG
    Serial.print("notifyDccMsg: ");
    for (uint8_t i = 0; i < Msg->Size; i++)
    {
        Serial.print(Msg->Data[i], HEX);
        Serial.write(' ');
    }
    Serial.println();
#endif

    lastDCCTime = millis();

}


// This call-back function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read
// So we will just turn the motor on for 8ms and then turn it off again.
void notifyCVAck(void) {
#ifdef DEBUG_DCC_ACK
    Serial.println("notifyCVAck");
#endif

    digitalWrite(DCC_ACK_PIN, HIGH);

    delay(10);

    digitalWrite(DCC_ACK_PIN, LOW);
    //  delay(50);

}

void initDecoder() {
#ifdef DEBUG_PRINT   
    Serial.println("initDecoder");
#endif // DEBUG_PRINT


    Aux[AUX1].SetBrigthness(Dcc.getCV(CV_AUX1_BRIGHTNESS));
    Aux[AUX2].SetBrigthness(Dcc.getCV(CV_AUX2_BRIGHTNESS));

    Aux[AUX1].SetAutoOff(Dcc.getCV(CV_AUX1_AUTO_OFF));
    Aux[AUX2].SetAutoOff(Dcc.getCV(CV_AUX2_AUTO_OFF));
    Aux[AUX3].SetAutoOff(Dcc.getCV(CV_AUX3_AUTO_OFF));
    Aux[AUX4].SetAutoOff(Dcc.getCV(CV_AUX4_AUTO_OFF));
    Aux[AUX5].SetAutoOff(Dcc.getCV(CV_AUX5_AUTO_OFF));

    Aux[AUX1].SetFunction(Dcc.getCV(CV_AUX1_DIR), Dcc.getCV(CV_AUX1_MODE), Dcc.getCV(CV_AUX1_TIME_ON), Dcc.getCV(CV_AUX1_TIME_OFF));
    Aux[AUX2].SetFunction(Dcc.getCV(CV_AUX2_DIR), Dcc.getCV(CV_AUX2_MODE), Dcc.getCV(CV_AUX2_TIME_ON), Dcc.getCV(CV_AUX2_TIME_OFF));
    Aux[AUX3].SetFunction(Dcc.getCV(CV_AUX3_DIR), Dcc.getCV(CV_AUX3_MODE), Dcc.getCV(CV_AUX3_TIME_ON), Dcc.getCV(CV_AUX3_TIME_OFF));
    Aux[AUX4].SetFunction(Dcc.getCV(CV_AUX4_DIR), Dcc.getCV(CV_AUX4_MODE), Dcc.getCV(CV_AUX4_TIME_ON), Dcc.getCV(CV_AUX4_TIME_OFF));
    Aux[AUX5].SetFunction(Dcc.getCV(CV_AUX5_DIR), Dcc.getCV(CV_AUX5_MODE), Dcc.getCV(CV_AUX5_TIME_ON), Dcc.getCV(CV_AUX5_TIME_OFF));

    if (Dcc.getCV(CV_AUX1_MODE) == 3) {
        Aux[AUX1].SetAutoOff(Dcc.getCV(CV_AUX1_AUTO_OFF));
    }
    if (Dcc.getCV(CV_AUX2_MODE) == 3) {
        Aux[AUX2].SetAutoOff(Dcc.getCV(CV_AUX2_AUTO_OFF));
    }
    if (Dcc.getCV(CV_AUX3_MODE) == 3) {
        Aux[AUX3].SetAutoOff(Dcc.getCV(CV_AUX3_AUTO_OFF));
    }
    if (Dcc.getCV(CV_AUX4_MODE) == 3) {
        Aux[AUX4].SetAutoOff(Dcc.getCV(CV_AUX4_AUTO_OFF));
    }
    if (Dcc.getCV(CV_AUX5_MODE) == 3) {
        Aux[AUX5].SetAutoOff(Dcc.getCV(CV_AUX5_AUTO_OFF));
    }

    // Read the current CV values for vStart and vHigh
    vStart = Dcc.getCV(CV_VSTART);
    vHigh = Dcc.getCV(CV_VHIGH);

    analogMode = Dcc.getCV(CV_ANALOG_MODE);
    AllwaysOn = Dcc.getCV(CV_ALLWAYS_ON);

    lastAUXState = Dcc.getCV(CV_AUX_State);
    saveAUXState = bool(Dcc.getCV(CV_AUX_Save));
}

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


#ifdef DEBUG_PRINT
    Serial.begin(115200);
    Serial.println(F("NMRA DCC Multifunction Decoder"));
    Serial.print(F("  CV1:  "));
    Serial.println(EEPROM.read(1));
    Serial.print(F("  CV7:  "));
    Serial.println(EEPROM.read(7));
    Serial.print(F("  CV8:  "));
    Serial.println(EEPROM.read(8));
    Serial.print(F("  CV29: "));
    Serial.println(EEPROM.read(29));

#endif

#ifdef EEPROM_Clear
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 255);
    }

#ifdef DEBUG_PRINT
    Serial.println(F("eeprom resetet"));
#endif

#endif

#ifdef __AVR_ATmega328P__
    Aux[AUX1].SetPinPWM(LED_PIN_FWD); // AUX 1
    Aux[AUX1].off();

    Aux[AUX2].SetPinPWM(LED_PIN_REV); // AUX 2
    Aux[AUX2].off();

    Aux[AUX3].SetPin(LED_PIN_AUX3);
    Aux[AUX3].off();

#ifdef fDecoder
    Aux[AUX4].SetPin(LED_PIN_AUX4);
    Aux[AUX4].off();

    Aux[AUX5].SetPin(LED_PIN_AUX5);
    Aux[AUX5].off();
#endif // fDecoder


#elif defined(__AVR_ATtiny85__)
    // This section defines the Arduino ATTiny85 Pins to use 
    Aux[AUX1].SetPinPWM(LED_PIN_FWD); // AUX 1
    Aux[AUX1].off();

    Aux[AUX2].SetPinPWM(LED_PIN_REV); // AUX 2
    Aux[AUX2].off();

    Aux[AUX3].SetPin(LED_PIN_AUX3);
    Aux[AUX3].off();

#ifdef fDecoder
    Aux[AUX4].SetPin(LED_PIN_AUX4);
    Aux[AUX4].off();
#endif // fDecoder

#endif 


    // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
#ifdef digitalPinToInterrupt
    Dcc.pin(DCC_PIN, 1);
#else
    Dcc.pin(0, DCC_PIN, 1);
#endif // digitalPinToInterrupt

    // Call the main DCC Init function to enable the DCC Receiver
    Dcc.init(MAN_ID_DIY, 10, FLAGS_MY_ADDRESS_ONLY | FLAGS_AUTO_FACTORY_DEFAULT, 0);

#ifdef FactoryDefaults
    notifyCVResetFactoryDefault();
#endif // FactoryDefaults

    if (FactoryDefaultCVIndex == 0) {
        initDecoder();
    };

    saveAUXState = bool(Dcc.getCV(CV_AUX_Save));

    if (saveAUXState) {
#ifdef DEBUG_FUNCTIONS
        Serial.print(F("Saved AUX State: "));
        Serial.print(lastAUXState, BIN);

        Serial.println();
#endif
        // Set the status back
        for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
            Aux[i].newState(BIT_CHECK(lastAUXState, i));
        }
    }



#ifdef sDecoder
    // setup for sound decoder

    mySoftwareSerial.begin(9600);

#ifdef DEBUG_PRINT
    Serial.println();
    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
#endif // DEBUG_PRINT

    if (!myDFPlayer.begin(mySoftwareSerial, false)) {  //Use softwareSerial to communicate with mp3.

#ifdef DEBUG_PRINT
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
#endif // DEBUG_PRINT

        while (true);
    }

#ifdef DEBUG_PRINT
    Serial.println(F("DFPlayer Mini online."));
#endif // DEBUG_PRINT

    myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

    // set volume
    uint8_t volume = Dcc.getCV(CV_SND_VOLUME);
    if (volume > 30)
        volume = 30;
    myDFPlayer.volume(volume);  //Set volume value (0~30).

    // set EQ
    uint8_t eq = Dcc.getCV(CV_SND_EQ);
    if (eq > 5)
        eq = 5;
    myDFPlayer.EQ(eq);

    //----Set device we use SD as default----
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

#endif // sDecoder

#ifdef DEBUG_FUNCTIONS
    Serial.println(F("leave setup"));
#endif // DEBUG_FUNCTIONS


}

void loop() {

    // call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
    Dcc.process();


    // If we do not receive a DCC signal within a certain time, then we are in analog mode
    if (millis() - lastDCCTime >= currentDCCInterval) {
#ifdef DEBUG_PRINT
        Serial.println(F("Analog mode"));
#endif    
       for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
           Aux[i].newState(BIT_CHECK(analogMode, bitmask[i]));
       }
    }

    // Handle direction and headlight changes
    for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
        Aux[i].process();
    }

    // Handle resetting CVs back to Factory Defaults
    if (FactoryDefaultCVIndex && Dcc.isSetCVReady()) {
        FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array 
        Dcc.setCV(FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);

        // Is this the last Default CV to set? if so re-init zDecoder
        if (FactoryDefaultCVIndex == 0)
            initDecoder();
    }


#ifdef sDecoder
    if (lastVolume != newVolume) {
        lastVolume = newVolume;
        myDFPlayer.volume(lastVolume);
    }

    if (lastSound != newSound) {
        lastSound = newSound;
        if ((lastSound >= 1) && (lastSound <= 100)) {
            myDFPlayer.play(lastSound);

        }
        else if ((lastSound >= 101) && (lastSound <= 200)) {
            myDFPlayer.advertise(lastSound);

        }
        else if ((lastSound >= 201) && (lastSound <= 240)) {
            myDFPlayer.loop(lastSound);

        }
    }

#endif // sDecoder

}
