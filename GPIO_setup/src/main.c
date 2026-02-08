#include <stdio.h>
#include "6190.h"

void setup(){
    /* Setup function to configure GPIO pins and real-time clock.  Today, the real-time
    clock has already been configured for you. */

    timerSetup(); // Initialize timer driven by real-time clock.

    pinSetup(LED1, GPIO_OUTPUT); // Configure pin connected to LED1 as output
    pinSetup(LED2, GPIO_OUTPUT); // Configure pin connected to LED2 as output

    pinSetup(BTNL, GPIO_INPUT); // Configure pin connected to BTNL as input
    pinSetup(BTNR, GPIO_INPUT); // Configure pin connected to BTNR as input
}

void app_main() {
    /* Main function, your program starts here. Any functions
        you call here must be defined above or in 6s077.h. */

    setup();

    // pinWrite(LED1, 1);  // Turn LED1 on
    // pinWrite(LED2, 0);  // Turn LED2 off

    while(1){
        // main loop of our program

        int x = pinRead(BTNL);  // Read value of BTNL
        int y = pinRead(BTNR);  // Read value of BTNR

        //implement XOR gate on LED1
        if (x != y){
            pinWrite(LED1, 1);  // Turn LED1 on
        } else if (x == y){
            pinWrite(LED1, 0);  // Turn LED1 off
        }

        //implement AND gate on LED2
        if (x == 0 && y == 0){
            pinWrite(LED2, 1);
        } else {
            pinWrite(LED2, 0);
        }
    }
}
