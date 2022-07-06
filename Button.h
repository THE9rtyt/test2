#pragma once

class Button {
public:
    void init(uint8_t pinNum);
    bool update(uint32_t update_time); // used to ask if the button has been pressed

private:
    uint8_t pin;
    int8_t current_state, last_state, timeout;
};