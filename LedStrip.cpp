#include "LedStrip.h"

#define LIGHT_UPDATE 10.0; //time between light color updates
const double Pulse_Interval = 2000; //length of pulse/rainbow from color on,off,on

#define BRIGHTNESS_MIN .01
#define BRIGHTNESS_MAX .95
#define AMPLITUDE (BRIGHTNESS_MAX-BRIGHTNESS_MIN)/2 //used to align the cos() to MIN/MAX brightness
#define SHIFT (BRIGHTNESS_MAX+BRIGHTNESS_MIN)/2 //used to align the cos() to MIN/MAX brightness
#define OFFSET (TWO_PI)/3
#define OFFSET_INV OFFSET*-1

//public functions

void LedStrip::init(rgb_t& pinSetup) {
    pin = pinSetup;
    status.color = Color::red;
    status.mode = Mode::solid;
    update_color();
    brightness = 1;
    animation_step = 0;

	pinMode(pin.r, OUTPUT);
	pinMode(pin.g, OUTPUT);
    pinMode(pin.b, OUTPUT);
	pinMode(pin.w, OUTPUT);

    set_color();
};

void LedStrip::update(const uint32_t& current_time) {
	if (current_time >= next_update_time) {
        switch(status.mode){
            case Mode::pulsing:
                update_pulse();
                update_color();
                break;
            case Mode::solid:
                brightness = BRIGHTNESS_MAX;
                update_color();
                break;
            case Mode::rainbow:
                brightness = BRIGHTNESS_MAX;
                update_rainbow();
                break;
            default:
                color = {0,0,0,0};
                status.mode = Mode::off;
                break;
        }
        
        set_color();
        next_update_time = current_time + LIGHT_UPDATE;
    }
};

void LedStrip::next_color() {
    switch(status.mode){
        case Mode::solid:
        case Mode::pulsing:
            status.color = static_cast<Color>(status.color+1);
        default:
            break;
    }
}

void LedStrip::next_mode() {
    status.mode = static_cast<Mode>(status.mode+1);
}

//private functions
void LedStrip::update_color() {
	switch(status.color){
		case Color::yellow:
			color = {255,255,0,0};
			break;
		case Color::green:
			color = {0,255,0,0};
			break;
		case Color::cyan:
			color = {0,255,255,0};
			break;
		case Color::blue:
			color = {0,0,255,0};
			break;
		case Color::magenta:
			color = {255,0,255,0};
			break;
		case Color::white:
			color = {0,0,0,255};
			break;
        default:
            color = {255,0,0,0};
            status.color = Color::red;
			break;
	}
}

void LedStrip::animation_update() {
    animation_step = animation_step + LIGHT_UPDATE;

	if (animation_step >= Pulse_Interval) {
		animation_step = 0;
	}
}

double LedStrip::wave_gen(const double& offset) {
    return cos(TWO_PI / Pulse_Interval * animation_step + offset);
}

void LedStrip::update_pulse() {
    animation_update();
    brightness = AMPLITUDE * wave_gen(0) + SHIFT;
}

double LedStrip::wave_rainbow(const double& offset) {
    double wave = wave_gen(offset);

    return 127.5 * wave+127.5;
}

void LedStrip::update_rainbow() {
    animation_update();
    color.r = wave_rainbow(0);
    color.g = wave_rainbow(OFFSET_INV);
    color.b = wave_rainbow(OFFSET);

}

void LedStrip::set_color() {
	analogWrite(pin.r,  color.r*brightness);
	analogWrite(pin.g,	color.g*brightness);
	analogWrite(pin.b, 	color.b*brightness);
	analogWrite(pin.w, 	color.w*brightness);
}