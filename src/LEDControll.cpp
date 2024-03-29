// 
// 
// 

#include "LEDControll.h"

LED::LED() :
	m_mode(1),
	m_lastState(0),
	m_newState(0),
	m_State(false),
	m_usePWM(false),
	m_Brightness(255),
	m_currentInterval(0) {

	pinMode(m_pin, OUTPUT);
	off();
}

LED::LED(uint8_t Pin_) :
	m_pin(Pin_),
	m_mode(1),
	m_lastState(0),
	m_newState(0),
	m_State(false),
	m_usePWM(false),
	m_Brightness(255),
	m_currentInterval(0) {

	pinMode(m_pin, OUTPUT);
	off();
}

LED::LED(uint8_t Pin_, uint8_t Brightness_) :
	m_pin(Pin_),
	m_mode(1),
	m_lastState(0),
	m_newState(0),
	m_State(false),
	m_usePWM(false),
	m_Brightness(Brightness_),
	m_currentInterval(0) {

	pinMode(m_pin, OUTPUT);
	off();
}

void LED::process() {

	if (m_lastDirection != m_newDirection) {
		m_lastDirection = m_newDirection;
	}

	if (m_lastState != m_newState) {
		m_lastState = m_newState;
		m_startTime = 0;
		m_State = 0;
	}

	switch (m_mode) {

	case 1:	// On / Off
		// DIR_BIT_REV
		if ((m_lastDirection == m_DIR_Mode) && (state())) {
			off();
		}
		// DIR_BIT_FWD
		else if ((m_lastDirection != m_DIR_Mode) && (state())) {
			on();
		}
		// DIR_BIT_BOTH
		else if ((m_DIR_Mode == DIR_BIT_BOTH) && (state())) {
			on();
		}
		// off
		else if (!(state())) {
			off();
		}
		break;

	case 2: // blinker
		// do nothing if not enabled
		if (!(state())) {
			off();
			return;
		}

		// do nothing if wrong direction
		if (!((m_lastDirection == m_DIR_Mode) || (m_DIR_Mode == DIR_BIT_BOTH))) {
			off();
			return;
		}


		// if time to do something, do it
		if (millis() - m_startTime >= m_currentInterval) {
			if (m_State == 0) {
				on();
				m_currentInterval = m_OnTime;
			}
			else {
				off();
				m_currentInterval = m_OffTime;
			}
			m_State = !m_State;
		} // end of if


		break;

	case 3:
		// DIR_BIT_REV
		if ((m_lastDirection == m_DIR_Mode) && (state())) {
			// off();
		}
		// DIR_BIT_FWD
		else if ((m_lastDirection != m_DIR_Mode) && (state())) {
			if (m_State == 0) {
				on();
				m_State = 1;
			}

		}
		// DIR_BIT_BOTH
		else if ((m_DIR_Mode == DIR_BIT_BOTH) && (state())) {
			if (m_State == 0) {
				on();
				m_State = 1;
			}
		}
		// off
		else if (!(state())) {
			// off();
		}

		// disable if time
		if (millis() - m_startTime >= m_currentInterval) {
			off();
		}
		break;

	default:
		m_mode = 1;

	}

}

void LED::on() {
	m_startTime = millis();
	// digitalWrite(m_pin, HIGH);


	if (m_usePWM) { // Pin's are PWM capable
		analogWrite(m_pin, m_Brightness);

	}
	else {
		digitalWrite(m_pin, HIGH);
	}


}

void LED::off() {
	m_startTime = millis();
	digitalWrite(m_pin, LOW);
}

void LED::newState(uint8_t State_) {
	m_newState = State_;
	//Serial.print("LED::newState; PIN: ");
	//Serial.print(m_pin);
	//Serial.print(", State: ");
	//Serial.println(State_);
}

void LED::newDirection(uint8_t DIR_) {
	m_newDirection = DIR_;
}

bool LED::state() {
	return m_lastState == 1;
}

void LED::SetPin(uint8_t Pin_) {
	m_pin = Pin_;
	pinMode(m_pin, OUTPUT);
	digitalWrite(m_pin, m_lastState);
}

void LED::SetPinPWM(uint8_t Pin_) {
	m_usePWM = true;
	SetPin(Pin_);
}

void LED::SetBrigthness(uint8_t Brightness_) {
	if (Brightness_ == 0)
		Brightness_ = 1;

	m_Brightness = Brightness_;
}

void LED::SetAutoOff(uint8_t Delay_) {
	m_currentInterval = Delay_ * 10;
}

void LED::SetFunction(uint8_t DIR_, uint8_t Mode_, uint8_t On_, uint8_t Off_) {

	m_DIR_Mode = DIR_ - 1;

	m_OnTime = On_ * 10;
	m_OffTime = Off_ * 10;

	m_mode = Mode_;
}

