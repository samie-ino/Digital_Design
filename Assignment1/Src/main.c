
/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Samone-Brandon
 * @brief          : Main program body
 ******************************************************************************



#define GPIOA_MODER (*(volatile unsigned int *)0x50000000)
#define GPIOA_ODR 	(*(volatile unsigned int *)0x50000014)
#define RCC_IOPENR  (*(volatile unsigned int *)0x40021034)

void wait_ticks(int ticks)
{
	for (volatile int count=0; count<ticks; count++){
		//do nothing
	}
}

int main(void)
{
	RCC_IOPENR |= (1<<0); // enable GPIO
	GPIOA_MODER &= ~(2<<(8*2));// configure PA8 as GPIO
	GPIOA_MODER |= (1<<(8*2)); // configure PA8 as output

	for(;;){
		GPIOA_ODR |= (1<<8); //PA8 high
		wait_ticks(266895);
		GPIOA_ODR &= ~(1<<8); // PA8 low
		wait_ticks(400371);
	}
}

*/


#define GPIOA_MODER (*(volatile unsigned int *)0x50000000)
#define GPIOA_ODR   (*(volatile unsigned int *)0x50000014)
#define RCC_IOPENR  (*(volatile unsigned int *)0x40021034)

const float m = 0.7494f;
const float b = 2.424f;

void wait_ms(int ms)
{
    int ticks;

    // Convert milliseconds to loop count
    ticks = (int)((1000.0f * ms - b) / m);

    for (volatile int count = 0; count < ticks; count++)
    {
        // do nothing
    }
}

int main(void)
{
     RCC_IOPENR |= (1 << 0);      // Enable GPIOA clock

    GPIOA_MODER &= ~(2 << (8 * 2)); // Configure PA8 as GPIO
    GPIOA_MODER |=  (1 << (8 * 2)); // Configure PA8 as output

    while (1)
    {
        GPIOA_ODR |= (1 << 8);   // PA8 high
        wait_ms(200);            // 200 ms high time

        GPIOA_ODR &= ~(1 << 8);  // PA8 low
        wait_ms(300);            // 300 ms low time
    }
}
