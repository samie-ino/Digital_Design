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

	// Difference between player 1 & 2
	for (int i = 0; i < 9; i++) {

	    if (player_board[i] == 1) {
	        // Player A owns this position
	    }

	    else if (player_board[i] == 2) {
	        // Player B owns this position
	    }
	}




	// turn on pa15
	GPIOA_MODER &= ~(3<<15*2); // Clear Led P15
	GPIOA_MODER |= (1<<(15*2)); // Set P15 to output

	GPIOA_ODR |= (1<<15); // Turn on led P15

	// Switches
	GPIOA_MODER &= ~(3<<(12*2)); // PA 12
	GPIOA_MODER &= ~(3<<(5*2)); // PA 5
	GPIOA_MODER &= ~(3<<(4*2)); // PA 4



	GPIOA_PUPDR &= ~(3<<(5*2)); // Clear PA 5
	GPIOA_PUPDR |= (2<<(5*2)); //Set PA5 Pulldown
	GPIOA_PUPDR &= ~(3<<(12*2)); // Clear PA 12
	GPIOA_PUPDR |= (2<<(12*2)); //Set PA12 Pulldown
	GPIOA_PUPDR &= ~(3<<(4*2)); // Clear PA 4
	GPIOA_PUPDR |= (2<<(4*2)); //Set PA4 Pulldown








	}
