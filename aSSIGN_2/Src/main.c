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
while (game_on){

// HORIZONTAL CONDITIONS - PLAYER A
// If player A == 9, 10, 15 player A wins

// If player A == 6, 7, 8 player A wins

// If player A == 5, 12, 11 player A wins

// VERTICAL  CONDITIONS - PLAYER A
// If player A == 9, 6, 5 player A wins

// If player A == 10, 7, 12 player A wins

// If player A == 15, 8, 11 player A wins

// DIAGNOL CONDITIONS
// If player A == 9, 10, 15 player A wins
// If player A == 6, 7, 8 player A wins
// If player A == 5, 12, 11 player A wins







	}
