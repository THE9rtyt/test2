// this is kind of fun

//includes
#include "Button.h"
#include <Arduino.h>

//pin config
#define RED 10
#define GREEN 11
#define BLUE 12
#define WHITE 9

#define COLOR_BUTTON A0
#define MODE_BUTTON A1

//LED vars
struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t w;
};

double brightness = 1;

RGB color = {255,0,0,0};
uint8_t mode = 0;

#define BRIGHTNESS_MIN .01
#define BRIGHTNESS_MAX .95
#define AMPLITUDE (BRIGHTNESS_MAX-BRIGHTNESS_MIN)/2 //used to align the cos() to MIN/MAX brightness
#define SHIFT (BRIGHTNESS_MAX+BRIGHTNESS_MIN)/2 //used to align the cos() to MIN/MAX brightness

//time managment

const double Light_Update = 10; // length of time between light updates
const double Pulse_Interval = 2000; // length of pulse from on -> off -> on
uint32_t next_light_update = 0; // time until next light update
double pulse_step = 0; // current step the wave is in

uint32_t previous_time = 0;

//debouncing vars
Button Color_Button;

double pulse_update() {
	return AMPLITUDE * cos(TWO_PI / Pulse_Interval * pulse_step) + SHIFT;
}

uint8_t step_update() {
	pulse_step = pulse_step + Light_Update;

	if (pulse_step >= Pulse_Interval) {
		pulse_step = 0;
	}
}

RGB get_color(uint8_t mode_color) {
	color = {0,0,0,0};
	switch(mode_color){
		case 0:
			color.r = 255;
			break;
		case 1:
			color.r = 255;
			color.g = 255;
			break;
		case 2:
			color.g = 255;
			break;
		case 3:
			color.g = 255;
			color.b = 255;
			break;
		case 4:
			color.b = 255;
			break;
		case 5:
			color.b = 255;
			color.r = 255;
			break;
		case 6:
			color.w = 255;
			break;
		//case 7 is off
	};

	return color;
}

void set_color(RGB color, double brightness) {
	analogWrite(RED,  	color.r*brightness);
	analogWrite(GREEN,	color.g*brightness);
	analogWrite(BLUE, 	color.b*brightness);
	analogWrite(WHITE, 	color.w*brightness);
}

void setup() {
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
	Serial.begin(9600);

	set_color(color, brightness);

    Color_Button.init(COLOR_BUTTON);
}

void loop() {	
	uint32_t current_time = millis();

	//main button check loop
	if (Color_Button.update(current_time - previous_time)) { //main button check loop
		mode++;
		if (mode > 7) mode = 0;
		color = get_color(mode);
	}

	//main light update loop
	if (current_time > next_light_update) {
		step_update();
		brightness = pulse_update();
		set_color(color, brightness);

		next_light_update = current_time + Light_Update;
	}
	
	//set vars for next loop
	previous_time = current_time;
}