#include <stdio.h>
#include "6190.h"
#include "ascii.h"

void setup(){

    timerSetup();   // Configure timer
    setupDisplay(); // Configure display
    eraseBuffer();  // Erase random data from display
    drawBuffer();   // Display contents of buffer on LED array
}


int messageLength(char* message){
    //////////////////////////////////////////
    // TO-DO: Complete messageLength()      //
    //////////////////////////////////////////
    int counter = 0;
    while ((*message) != '\0'){
        message += 1;
        counter ++;
    }
    return counter;
}


void fillScreenBuffer(char * message, int total_offset){

    //////////////////////////////////////////
    // TO-DO: Complete fillScreenBuffer()   //
    //////////////////////////////////////////
    int size = 5;
    if (total_offset % 8 == 0){
        size = 4;
    }
    message += total_offset / 8;
    for (int i=total_offset / 8; i<total_offset / 8 + size; i++){
        if ((*message) == '\0'){
            break;
        }   
        else{
            for (int j=0; j<8; j++){
                if ((i==total_offset / 8 + size-1) && size == 5){
                    screen_buffer[j] |= (ascii[(int)(*message)][j] >> (8-total_offset%8));
                }
                else screen_buffer[j] |= (ascii[(int)(*message)][j] << (8*(3-i)+total_offset));
            }
        }
        message += 1;
    }
    drawBuffer();
} 


void app_main(){
    setup();

    char message[] = "     HELLO BUNNY I WUV UUUUU     ";
    int len = messageLength(message);  // TASK 1: CALCULATE MESSAGE LENGTH
    int offset = 0;
    while(1){
        if (offset == len * 8){  
            offset = 0; // Reset if we have reached the end of the message
        } else {
            eraseBuffer();  // Clear screen buffer
            fillScreenBuffer(message, offset); // TASK 2: FILL SCREEN BUFFER WITH CORRECT DATA
            drawBuffer();   // Transmit screen buffer data to screen
            offset++;        // Move forward
        }

        int start = millis();            // Get "start" time stamp
        while(millis() - start < 100);   // Wait until (current time stamp - "start") >= 100ms   
    }
}
