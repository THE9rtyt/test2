#include "Button.h"

#define DEFAULT_TIMEOUT 90;

void Button::init(uint8_t pinNum) {
    pin = pinNum;
    current_state = HIGH;
    last_state = HIGH;
    timeout = 0;
    pinMode(pin, INPUT_PULLUP);
}

bool Button::update(uint32_t update_time) {
    current_state = digitalRead(pin);
    if (last_state == LOW && timeout <= 0) {
		timeout = DEFAULT_TIMEOUT;
        return true;
    }
    
	if (current_state == HIGH && timeout > 0) {
		timeout = timeout - (update_time);
	}

    last_state = current_state;

    return false;
}