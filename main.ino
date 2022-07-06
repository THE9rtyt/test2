// this is kind of fun

//includes
#include <Arduino.h>
#include "Button.h"
#include "LedStrip.h"

//pin config
#define RED 10
#define GREEN 11
#define BLUE 12
#define WHITE 9

#define COLOR_BUTTON A0
#define MODE_BUTTON A1

//time managment
uint32_t previous_time = 0;

//define Objects
Button Color_Button, Mode_Button;
LedStrip Main_Strip;

void setup() {
	Serial.begin(9600);
    
    Color_Button.init(COLOR_BUTTON);
    Mode_Button.init(MODE_BUTTON);

    rgb_t pin_setup = {RED,GREEN,BLUE,WHITE};
    Main_Strip.init( pin_setup );
}

void loop() {	
	uint32_t current_time = millis();
    uint32_t time_since_last_loop = current_time - previous_time;

	//main button check loop
	if (Color_Button.update(time_since_last_loop)) { //main button check loop
		Main_Strip.next_color();
	}
    
    if (Mode_Button.update(time_since_last_loop)) {
        Main_Strip.next_mode();
    }

	//main light update loop
	Main_Strip.update(current_time);
	
	//set vars for next loop
	previous_time = current_time;
}