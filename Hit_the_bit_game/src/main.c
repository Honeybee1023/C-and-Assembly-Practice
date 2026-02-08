#include <stdio.h>
#include "6190.h"

const int TIME = 1000; // loop time
int switch_pins[] = {SW0,SW1,SW2,SW3,SW4,SW5,SW6,SW7};

// TASK 1:
// Get updateLeds working below
void updateLeds(int *leds){
    int right_bit = *leds & 0b1;
    *leds >>= 1;
    *leds = *leds | (right_bit << 7);
}

// TASK 2:
// Get getInputs working below
int getInputs(){
    int bit = 0b0;
    for (int i = 0; i<8; i++){
        bit = bit | (pinRead(switch_pins[i]) << i);
    }
    return bit;
}

// positions the 8-long led array into the larger screen buffer:
void drawLedArray(int leds){
    screen_buffer[7] = leds<<8;
    drawBuffer();
}

void app_main(){
    timerSetup();
    setupDisplay();
    for(int i=0; i<8; i++){
        pinSetup(switch_pins[i],GPIO_INPUT);
    }
    eraseBuffer();
    int leds = 0x10;                           // initial state of LEDs
    int old_switches =0;                       // for remembering switches
    drawLedArray(leds);                        // draw initial state on LED array
    long timer = millis(); 
    while(1){
        while(millis()-timer < TIME){          // wait for TIME milliseconds
            int new_switches = getInputs();
            if (new_switches != old_switches){
                leds = leds ^ new_switches;                   // TASK 3 (update so new_switches alters leds!)
                drawLedArray(leds);            // draw immediately
                old_switches = new_switches;   // remember what the switches were.
                break;
            }
        }
        while(millis()-timer < TIME);           // wait for remainder of loop time
        timer = millis();                       // update and remember
        updateLeds(&leds);                      // update LED array  
        drawLedArray(leds);        
    }
}