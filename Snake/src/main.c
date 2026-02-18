#include <stdio.h>
#include <stdint.h>
#include "6190.h"

// Constants
#define SCREEN_ROWS 8
#define SCREEN_COLS 32
enum snake_direction { up, down, left, right };

// Setup for random value generator
#define RANDOM_VALUE_ADDR 0x600260B0 
#define RTC_CNTL_CLK_CONF_REG 0x60008070

void setupRandom(){
    int *rtc_cntl_clk_conf_rg = (int*) RTC_CNTL_CLK_CONF_REG;
    *rtc_cntl_clk_conf_rg |= (1 << 10);
}


// Snake struct definition
struct Snake {
    uint8_t body[SCREEN_COLS*SCREEN_ROWS]; // can get as large as entire screen in theory
    uint8_t direction;
    uint8_t length; 
};


void setup(){
    timerSetup();
    setupRandom();
    setupDisplay();
    eraseBuffer();
    int snake_buttons[5] = {BTNF, BTNU, BTND, BTNR, BTNL};
    for (int i = 0; i < 5; i++){
        pinSetup(snake_buttons[i], GPIO_INPUT);
    }
}


uint8_t getX(uint8_t location){
    /* Function that returns the x coordinate from an 8-bit value containing both x and y coordinates.
    Arguments:
        location: an unsigned 8-bit value representing a location on an 8x32 gameboard.
    Returns:
        uint8_t representing the x-coordinate (0-31) encoded in the upper 5 bits of the argument.
    */

    return location >> 3;

}


uint8_t getY(uint8_t location){
    /* Function that returns the y coordinate from an 8-bit value containing both x and y coordinates.
    Arguments:
        location: an unsigned 8-bit value representing a location on an 8x32 gameboard.
    Returns:
        uint8_t representing the y-coordinate (0-7) encoded in the lower 3 bits of the argument.
    */

    return location & 7;

}


void setX(uint8_t *location, uint8_t new_x){
    /* Function that updates the upper 5 bits of the value stored at location to be equal to new_x.
    Arguments:
        uint8_t *location: a pointer to an unsigned 8-bit value representing a location on an 8x32 gameboard.
        uint8_t new_x: an unsigned integer representing the value (0-31) that we'd like to set for the x location.
    */
    for (int i=0; i< 5; i++){
        if (((new_x>>i)&1) == 0){
            *location &= ~(1 << (3+i));
        }
        else{
            *location |= (1 << (3+i));
        }
    }
}


void setY(uint8_t *location, uint8_t new_y){
    /* Function that updates the lower 3 bits of the value stored at location to be equal to new_y.
    Arguments:
        uint8_t *location: a pointer to an unsigned 8-bit value representing a location on an 8x32 gameboard.
        uint8_t new_y an unsigned integer representing the value (0-7) that we'd like to set for the y location.
    */

    *location &= 248;
    new_y &= 7;
    *location |= new_y;
}


void setPixel(uint8_t location, uint8_t val){
    /* Function that sets the value of an individual pixel (LED) on the display.
    Arguments:
        location: The location of the LED to be turned on/off on the LED array
        val: The binary (0 or 1) value indicating if the LED at the location should be off (0) or on (1).
    */
    if (val == 1){
        screen_buffer[getY(location)] |= (1 << getX(location));
    }
    else{
        screen_buffer[getY(location)] &= ~(1 << getX(location));
    }
}


void drawBoard(struct Snake *snake, uint8_t food){
    /* Function to render the snake and food on the board on each given time-step.
    Arguments:
        struct Snake *snake: pointer to snake struct
        uint8_t food: location of food to render
    */

    eraseBuffer();
    setPixel(food, 1);
    for (int i=0; i<snake->length; i++){
        setPixel(snake->body[i], 1);
    }
    drawBuffer();
}



void updateSnake(struct Snake *snake){
    /* Function to update the snake on each time-step based on its direction.
    Arguments: 
        struct Snake *snake: pointer to snake struct
    */

    for(int i = snake->length - 1; i>0; i--){
        snake->body[i] = snake->body[i-1];
    }
    if(snake->direction == up){
        setY(&(snake->body[0]), getY(snake->body[0]) - 1);
    }
    else if (snake->direction == down){
        setY(&(snake->body[0]), getY(snake->body[0]) + 1);
    }
    else if (snake->direction == left){
        snake->body[0] += 8;
    }
    else {
        snake->body[0] -= 8;
    }
}


int generateFood(struct Snake *snake, uint8_t *food){
    /* Function to randomly generate food for the snake.
    Arguments: 
        struct Snake *snake: pointer to snake struct
        uint8_t *food: pointer to variable holding the food's location
    Returns:
        int: 0 if food does not conflict with the snake, 1 if it does.
    */

    int *random_value_ptr = (int*) RANDOM_VALUE_ADDR; // read this address to obtain a random integer value

    uint8_t value = *random_value_ptr;

    for(int i = snake->length - 1; i>0; i--){
        if (snake->body[i] == value){
            return 1;
        }
    }

    *food = *random_value_ptr;

    return 0;
}


int snakeAteFood(uint8_t *snake_body, uint8_t food){
	 /* Function to determine if the snake collided with (and ate) the food.
    Arguments: 
        uint8_t *snake_body: pointer to the snake body array
        uint8_t food: variable holding the food's location
    Returns:
        int: 0 if the snake did not eat the food, 1 if it did.
    */
    
    return (snake_body[0] == food);
}



uint8_t snakeCollisionCheck(struct Snake *snake){
    /* Function to determine if the snake collided with itself.
    Arguments: 
        struct Snake *snake: pointer to snake array
    Returns:
        int: 0 if the snake did not collide with itself, 1 if it did.
    */

    for(int i = snake->length - 1; i>0; i--){
        if (snake->body[i] == snake->body[0]){
            return 1;
        }
    }
    if (getX(snake->body[0]) == 0 && snake->direction == right){
        return 1;
    }
    if (getX(snake->body[0]) == 31 && snake->direction == left){
        return 1;
    }
    if (getY(snake->body[0]) == 0 && snake->direction == up){
        return 1;
    }
    if (getY(snake->body[0]) == 7 && snake->direction == down){
        return 1;
    }
    return 0;
}




void app_main() {
    setup(); // System setup

    // Snake initialization
    struct Snake snake;
    // COMMENT OUT FOR PART 10
    // snake.body[0] = 0;
    // snake.direction = left;
    // snake.length = 1;

    // UNCOMMENT FOR PART 10
    for (int i=0; i<3; i++){
        setX(&snake.body[i], 5);
        setY(&snake.body[i], 3-i);
    }
    snake.length = 3;
    snake.direction = left;

    uint8_t food = 0; // food in top right corner of the game board

    while(generateFood(&snake, &food) != 0); // Generate food initially
    drawBoard(&snake, food);

    while(1){        
        
        //////////////////////////////////////////////
        // INSERT BUTTON PRESS DETECTION LOGIC HERE //
        //////////////////////////////////////////////
        int btnu = pinRead(BTNU);
        int btnl = pinRead(BTNL);
        int btnr = pinRead(BTNR);
        int btnd = pinRead(BTND);
        int btnf = pinRead(BTNF);
        if (btnu == 0 && snake.direction != down){
            snake.direction = up;
        }
        if (btnl == 0 && snake.direction != right){
            snake.direction = left;
        }
        if (btnr == 0 && snake.direction != left){
            snake.direction = right;
        }
        if (btnd == 0 && snake.direction != up){
            snake.direction = down;
        }
        if (btnf == 0){
            setup(); // System setup

            // UNCOMMENT FOR PART 10
            for (int i=0; i<3; i++){
                setX(&snake.body[i], 5);
                setY(&snake.body[i], 3-i);
            }
            snake.length = 3;
            snake.direction = left;

            uint8_t food = 0; // food in top right corner of the game board

            while(generateFood(&snake, &food) != 0); // Generate food initially
            drawBoard(&snake, food);
        }
        
        updateSnake(&snake);
        drawBoard(&snake, food);

        if (snakeAteFood(snake.body, food) == 1){
            //////////////////////////////
            // TO-DO: "Grow" snake      // 
            //////////////////////////////
            snake.length += 1;
            updateSnake(&snake);

            // Generate food
            while (generateFood(&snake, &food) != 0); // generate food
        }

        if (snakeCollisionCheck(&snake) == 1){
            // Game ends
            eraseBuffer(); // Clear game board
            drawBuffer();
            while(1){
                int btnf = pinRead(BTNF);
                if (btnf == 0){
                    setup(); // System setup

                    // UNCOMMENT FOR PART 10
                    for (int i=0; i<3; i++){
                        setX(&snake.body[i], 5);
                        setY(&snake.body[i], 3-i);
                    }
                    snake.length = 3;
                    snake.direction = left;

                    uint8_t food = 0; // food in top right corner of the game board

                    while(generateFood(&snake, &food) != 0); // Generate food initially
                    drawBoard(&snake, food);
                    break;
                }
            }
        }

        // Code to limit frequency of loop so that gameplay is possible.
        // Put all game code above this point.
        int start = millis();
        while (millis() - start < 200); // One loop iteration every ~200ms
    }
}