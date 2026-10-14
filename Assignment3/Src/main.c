#define RCC_BASE   0x40021000
#define GPIOA_BASE 0x50000000

// RCC registers
#define RCC_CR       (*(volatile unsigned int *)(RCC_BASE + 0x00))
#define RCC_CFGR     (*(volatile unsigned int *)(RCC_BASE + 0x08))
#define RCC_PLLCFGR  (*(volatile unsigned int *)(RCC_BASE + 0x0C))
#define RCC_IOPENR   (*(volatile unsigned int *)(RCC_BASE + 0x34))

// GPIOA registers
#define GPIOA_MODER   (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER  (*(volatile unsigned int *)(GPIOA_BASE + 0x04))
#define GPIOA_OSPEEDR (*(volatile unsigned int *)(GPIOA_BASE + 0x08))
#define GPIOA_PUPDR   (*(volatile unsigned int *)(GPIOA_BASE + 0x0C))
#define GPIOA_AFRH    (*(volatile unsigned int *)(GPIOA_BASE + 0x24))

int main(void)
{
    // ------------------------------------------------
    // HSI16
    // ------------------------------------------------

    // Enable HSI16
    RCC_CR |= (1 << 8);

    // Wait until HSI16 is ready
    while ((RCC_CR & (1 << 10)) == 0)
    {
    }


    // ------------------------------------------------
    // PLL CONFIGURATION
    // 16 MHz / 1 * 11 / 8 = 22 MHz
    // ------------------------------------------------

    // Disable PLL before changing PLL configuration
    RCC_CR &= ~(1 << 24);

    // Wait until PLLRDY is cleared
    while ((RCC_CR & (1 << 25)) != 0)
    {
    }

    // Select HSI16 as PLL source
    RCC_PLLCFGR &= ~0b11;
    RCC_PLLCFGR |= 0b10;

    // PLLM = /1
    // Shortcut: /1 = 000, so clearing is enough
    RCC_PLLCFGR &= ~(0b111 << 4);

    // Clear PLLN bits 14:8
    RCC_PLLCFGR &= ~(0b1111111 << 8);

    // PLLN = 11
    RCC_PLLCFGR |= (0b0001011 << 8);

    // Clear PLLR bits 31:29
    RCC_PLLCFGR &= ~(0b111u << 29);

    // PLLR = /8
    RCC_PLLCFGR |= (0b111u << 29);


    // ------------------------------------------------
    // ENABLE PLL
    // ------------------------------------------------

    // Turn PLL on
    RCC_CR |= (1 << 24);

    // Wait until PLL is ready
    while ((RCC_CR & (1 << 25)) == 0)
    {
    }

    // Enable PLLR output
    RCC_PLLCFGR |= (1 << 28);


    // ------------------------------------------------
    // PA8 GPIO CONFIGURATION
    // ------------------------------------------------

    // Enable GPIOA peripheral clock
    RCC_IOPENR |= (1 << 0);

    // PA8: Clear MODE8 bits 17:16
    GPIOA_MODER &= ~(0b11 << 16);

    // PA8: Alternate Function mode = 10
    GPIOA_MODER |= (0b10 << 16);

    // PA8: Push-pull
    // Shortcut: Push-pull = 0, so clearing is enough
    GPIOA_OTYPER &= ~(1 << 8);

    // PA8: Clear OSPEED8 bits 17:16
    GPIOA_OSPEEDR &= ~(0b11 << 16);

    // PA8: High Speed = 10
    GPIOA_OSPEEDR |= (0b10 << 16);

    // PA8: No pull-up / no pull-down
    // Shortcut: 00, so clearing is enough
    GPIOA_PUPDR &= ~(0b11 << 16);

    // PA8: AF0 = MCO
    // Shortcut: AF0 = 0000, so clearing is enough
    GPIOA_AFRH &= ~0b1111;


    // ------------------------------------------------
    // MCO CONFIGURATION
    // ------------------------------------------------

    // Disable MCO selection while configuring it
    RCC_CFGR &= ~(0b1111 << 24);

    // MCOPRE = /1
    // Shortcut: /1 = 0000, so clearing is enough
    RCC_CFGR &= ~(0b1111u << 28);

    // MCOSEL = PLLRCLK
    RCC_CFGR |= (0b0101 << 24);


    // ------------------------------------------------
    // MAIN LOOP
    // ------------------------------------------------

    while (1)
    {
    }
}
