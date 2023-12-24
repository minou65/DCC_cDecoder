// 
// 
// 

#include "SoundControll.h"


void SoundSetup(){
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
}

void SoundLoop() {
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
}
