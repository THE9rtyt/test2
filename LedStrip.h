#pragma once

#include <Arduino.h>

enum Color: uint8_t {
    red,
    yellow,
    green,
    cyan,
    blue,
    magenta,
    white
};

enum Mode: uint8_t {
    off,
    solid,
    pulsing,
    rainbow
};

typedef struct Status {
    Color color;
    Mode mode;
} status_t;

typedef struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t w;
} rgb_t;

class LedStrip {
public:
    void init(rgb_t& pinNum); //initialize LED Strip
    void update(const uint32_t& current_time); //update the strip, provide the current time
    void next_color(); // function to increment to the next color option(if in a mode that supports it)
    void next_mode(); // function to increment to the next mode

private:
    rgb_t pin, color;
    status_t status;
    
    double brightness, animation_step;
    uint32_t next_update_time;

    void update_color(); //updates the color var to the lates mode.color
    void animation_update();
    double wave_gen(const double& offset);
    void update_pulse();
    double wave_rainbow(const double& offset);
    void update_rainbow();
    void set_color(); //sends the color output to the pins
};