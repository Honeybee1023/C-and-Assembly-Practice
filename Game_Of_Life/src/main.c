#include <stdio.h>
#include "6190.h"
#include <stdlib.h>

uint32_t temp_buffer[8]; //temporarily holds new board

void initializeLoaf(uint32_t* gb){
  gb[0] = 0;
  gb[1] = 0;
  gb[2] = 0x3 << 15;
  gb[3] = 0x9 << 14;
  gb[4] = 0x5 << 14;
  gb[5] = 0x1 << 15;
  gb[6] = 0;
  gb[7] = 0;
}

void initializeGlider(uint32_t* gb){
  gb[0] = 0;
  gb[1] = 0x7 << 1;
  gb[2] = 0x1 << 3;
  gb[3] = 0x1 << 2;
  for (int i = 4; i < 8; i++){
    gb[i] = 0;
  }
}

// Setup for random value generator
#define RANDOM_VALUE_ADDR 0x600260B0 
#define RTC_CNTL_CLK_CONF_REG 0x60008070
void setupRandom(){
    int *rtc_cntl_clk_conf_rg = (int*) RTC_CNTL_CLK_CONF_REG;
    *rtc_cntl_clk_conf_rg |= (1 << 10);
}

void initializeRandom(uint32_t* gb){
    int *r = (int *) RANDOM_VALUE_ADDR;
    for (int i =0; i<8; i++){
        gb[i] = *r;
        long start = millis();
        while(millis()-start < 100); 
    }
}

void updateBoard(uint32_t *current_board, uint32_t *new_board);

void app_main() {
    timerSetup();
    setupRandom();
    setupDisplay();
    eraseBuffer(screen_buffer);
    const int TIME = 100; //loop time.
    // Choose an initializer:
    //initializeLoaf(screen_buffer); 
    // initializeGlider(screen_buffer); 
    initializeRandom(screen_buffer); 
    while(1){
        long start = millis();
        while(millis()-start < TIME); //wait for TIME milliseconds
        drawBuffer();
        updateBoard(screen_buffer, temp_buffer);

        // copy game_board back into screen_buffer
        for (int i=0; i<8;i++){
           screen_buffer[i] = temp_buffer[i];
        }
    }
}