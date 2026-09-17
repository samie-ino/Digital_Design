//Define the GPIO's
#define GPIOA_BASE 0x50000000
#define GPIOB_BASE 0x50000400
#define GPIOA_MODER (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOB_MODER (*(volatile unsigned int *)(GPIOB_BASE + 0x00))

// Led's Output Data Register
#define GPIOA_ODR (*(volatile unsigned int *)(GPIOA_BASE + 0x14))
#define GPIOB_ODR (*(volatile unsigned int *)(GPIOB_BASE + 0x14))


// Switches Input Data Register
#define GPIOA_IDR (*(volatile unsigned int *)(GPIOA_BASE + 0x10))


// Switches Pull Down
#define GPIOA_PUPDR (*(volatile unsigned int *)(GPIOB_BASE + 0x0C))
#define RCC_IOPENR  (*(volatile unsigned int *)0x40021034)

int game_on = 1;
int player_A = 1;
int player_B = 2;

int portA_board[9] = {
		9, 10, 15,
		6 ,7 , 8,
		5 , 12, 11
			};
int portB_board[9] = {
		1, 7, 6,
		8 ,2 , 0,
		4 , 5, 9
			};
int player_board[9] ={0};

int winner_check(void) {
    // Check Player A win conditions using actual board positions
    if (
        (player_board[9] == player_A && player_board[10] == player_A && player_board[15] == player_A) ||
        (player_board[6] == player_A && player_board[7] == player_A && player_board[8] == player_A) ||
        (player_board[5] == player_A && player_board[12] == player_A && player_board[11] == player_A) ||
        (player_board[9] == player_A && player_board[6] == player_A && player_board[5] == player_A) ||
        (player_board[10] == player_A && player_board[7] == player_A && player_board[12] == player_A) ||
        (player_board[15] == player_A && player_board[8] == player_A && player_board[11] == player_A) ||
        (player_board[9] == player_A && player_board[7] == player_A && player_board[11] == player_A) ||
        (player_board[15] == player_A && player_board[7] == player_A && player_board[5] == player_A)
    ) {
        return 1; // player A wins
    }

    // Check Player B win conditions using actual board positions
    if (
        (player_board[1] == player_B && player_board[7] == player_B && player_board[6] == player_B) ||
        (player_board[8] == player_B && player_board[2] == player_B && player_board[0] == player_B) ||
        (player_board[4] == player_B && player_board[5] == player_B && player_board[9] == player_B) ||
        (player_board[1] == player_B && player_board[8] == player_B && player_board[4] == player_B) ||
        (player_board[7] == player_B && player_board[2] == player_B && player_board[5] == player_B) ||
        (player_board[6] == player_B && player_board[0] == player_B && player_board[9] == player_B) ||
        (player_board[1] == player_B && player_board[2] == player_B && player_board[9] == player_B) ||
        (player_board[6] == player_B && player_board[2] == player_B && player_board[4] == player_B)
    ) {
        return 2; // player B wins
    }

    // If no winner yet, keep the game running
    return 0;
}

void reset(){
}
void turnA (int p){
	// turn on for A
	GPIOA_MODER &= ~(3<<p*2); // Clear Led at p
	GPIOA_MODER |= (1<<(p*2)); // Set P to output
	GPIOA_ODR |= (1<<p); // Turn on led at P
}

void turnB (int p){
	// turn on for B
			GPIOB_MODER &= ~(3<<p*2);
			GPIOB_MODER |= (1<<(p*2));
			GPIOB_ODR |= (1<<p);
}


// player board code
int main(void) {
	// For GPIOA
	RCC_IOPENR |= (1<<0); // enable GPIO
	// For GPIOB
	RCC_IOPENR |= (1<<1); // enable GPIO

    reset();

    while (game_on) {
        // check board after each move
        int result = winner_check();

        if (result == 1) {
            // Player A wins
            reset();
        }
        else if (result == 2) {
            // Player B wins
            reset();
        }

        for (int i = 0; i < 9; i++) {
            // go through every position on the board
            if (player_board[i] == player_A) {
                // if this square belongs to player A, light the matching LED for A
                turnA(portA_board[i]);
            }
            else if (player_board[i] == player_B) {
                // if this square belongs to player B, light the matching LED for B
                turnB(portB_board[i]);
            }
            else {
                // if the square is empty, do nothing
                // no LED should be turned on for this position
            }
        }
    }

    return 0;
}
