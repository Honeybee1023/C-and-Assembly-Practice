#include <stdio.h>
#include "6190.h"

void setup(){
    timerSetup();

    pinSetup(LED1, GPIO_OUTPUT); // Configure LED1 GPIO pin as output
    pinSetup(LED2, GPIO_OUTPUT); // Configure LED2 GPIO pin output

    pinSetup(BTNC, GPIO_INPUT);  // Configure BTNC GPIO pin as input

    setupDisplay();
    eraseBuffer(screen_buffer);
    drawBuffer();
}

void app_main() {
    setup();

    // CODE TO TEST pinWrite 
    pinWrite(LED1, 1);
    pinWrite(LED2, 0);

    int evens = 1, odds = 0;
    while(1){

        // CODE TO TEST pinRead 
        int x = pinRead(BTNC);  // Read value of GPIO pin 7
        if (x == 1){
            printf("button not pressed\n");
        } else if (x == 0){
            printf("BUTTON PRESSED!\n");
        }

        // CODE TO TEST setPixel 
        evens ^= 1;
        odds ^= 1;

        for (int i = 0; i < 32; i = i + 1){
            for (int j = 0; j < 8; j = j + 1){
                if (j%2== 0){
                    setPixel(screen_buffer, i, j, evens);
                }
                else{
                    setPixel(screen_buffer, i, j, odds);
                }
            }
        }
       
        drawBuffer();
        long start = millis();
        while (millis() - start < 1000);

    }
}