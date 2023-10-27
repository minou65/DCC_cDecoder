// LEDControll.h

#ifndef _LEDCONTROLL_h
#define _LEDCONTROLL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#define DIR_BIT_FWD	0
#define DIR_BIT_REV	1
#define DIR_BIT_BOTH 2


class LED {
protected:
    uint8_t m_pin;
    uint8_t m_mode;
    uint8_t m_DIR_Mode;

    uint8_t m_newState;
    uint8_t m_lastState;
    uint8_t m_newDirection;
    uint8_t m_lastDirection;


    uint8_t m_Brightness;

    bool m_usePWM;
    bool m_State; // State machine

    uint16_t m_OnTime;
    uint16_t m_OffTime;

    unsigned long m_currentInterval;  // time till we change state
    unsigned long m_startTime;        // when we last changed state

public:
    LED();
    LED(uint8_t Pin_);
    LED(uint8_t Pin_, uint8_t Brightness_);

    virtual void process();
    void on();
    void off();

    void newState(uint8_t State_);

    void newDirection(uint8_t DIR_);
    bool state();

    void SetPin(uint8_t Pin_);
    void SetPinPWM(uint8_t Pin_);
    void SetBrigthness(uint8_t Brightness_);
    void SetAutoOff(uint8_t Delay_);
    void SetFunction(uint8_t DIR_, uint8_t Mode_, uint8_t On_, uint8_t Off_);


};


#endif

