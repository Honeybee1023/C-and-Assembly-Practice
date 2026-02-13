#include <stdio.h>
#include "6190.h"
#include "ascii.h"

int switch_pins[8] = {SW0, SW1, SW2, SW3, SW4, SW5, SW6, SW7}; 

void setup(){

    timerSetup();   // Configure timer
    setupDisplay(); // Configure display
    eraseBuffer();  // Erase random data from display
    drawBuffer();   // Display contents of buffer on LED array

    for (int i = 0; i < 7; i++){
        pinSetup(switch_pins[i], GPIO_INPUT);
    }
    pinSetup(BTNF, GPIO_INPUT);

}


char switchToBinary(){
    /* Function that converts input from 7 switches to a 7-bit unsigned integer value.
    Return:
        char: 8-bit value. Bits 0-6 correspond to switch inputs, bit 7 is 0.
    */

    char output = 0;
    for (int i=0; i<7; i++){
        output = output | (pinRead(switch_pins[i]) << i);  
    }
    return output;
}


void drawAsciiString(char* string_to_draw){
    /* Function that draws a string on the LED array using the mapping defined in ascii.h
    by filling in a buffer.

    Argument:
        char* string_to_draw: String to draw on the LED array.
    */
    eraseBuffer();
    for (int i=0; i<4; i++){
        if ((*string_to_draw) == '\0'){
            break;
        }   
        else{
            for (int j=0; j<8; j++){
                screen_buffer[j] |= (ascii[(int)(*string_to_draw)][j] << 8*(3-i));
            }
        }
        string_to_draw += 1;
    }
    drawBuffer();
}


void app_main(){
    setup();

    char ascii_input = 0;
    int last_btnf = 1;
    char arr[100];
    int arr_counter = 0; 
    
    while(1){
        ascii_input = switchToBinary();
        
        int btnf = pinRead(BTNF);        // read BTNF to obtain the current state of the button
        if (btnf == 0 && last_btnf == 1){                    
            printf("Integer value: 0x%x, ", ascii_input);   // printing this value using %x formatter gives a hexadecimal value...
            printf("ASCII character: %c\n", ascii_input);   // ... printing the same value with %c formatter gives a character
            arr[arr_counter] = ascii_input;
            arr_counter += 1;
            if (ascii_input == 0){
                printf("\n Final string: %.*s \n", arr_counter + 1, arr);
                drawAsciiString(arr);
                arr_counter = 0;
            }
        }
        last_btnf = btnf;                           
        
        // Code to slow down loop so that "bouncing" associated with a single button press doesn't 
        // look like more than one button press. Write all of your code in above this point in the loop.
        int start = millis();           // Get "start" time stamp
        while(millis() - start < 50);   // Wait until current time stamp - "start" is greater than 50ms
    }
}
