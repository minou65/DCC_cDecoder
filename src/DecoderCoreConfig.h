// DecoderCoreConfig.h

#ifndef _DecoderCoreConfig_h
#define _DecoderCoreConfig_h

// Define which type of decoder this is by setting exactly one of the following to 1
#define SOUND_FEATURE 0 // 0 = Sound decoder disabled, 1 = Sound decoder enabled
#define CAR_DECODER 1   // 0 = Car decoder disabled, 1 = Car decoder enabled
#define FUNC_DECODER 0  // 0 = Function decoder disabled, 1 = Function decoder enabled

// Uncomment to force CV Reset to Factory Defaults
//#define FactoryDefaults
//#define EEPROM_Clear

#endif

