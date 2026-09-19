// Define the GPIO's

#define GPIOA_BASE 0x50000000
#define GPIOB_BASE 0x50000400

#define GPIOA_MODER (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOB_MODER (*(volatile unsigned int *)(GPIOB_BASE + 0x00))

// LED's Output Data Register

#define GPIOA_ODR (*(volatile unsigned int *)(GPIOA_BASE + 0x14))
#define GPIOB_ODR (*(volatile unsigned int *)(GPIOB_BASE + 0x14))

// Switches Input Data Register

#define GPIOA_IDR (*(volatile unsigned int *)(GPIOA_BASE + 0x10))

// Switches Pull-Up / Pull-Down Register

#define GPIOA_PUPDR (*(volatile unsigned int *)(GPIOA_BASE + 0x0C))

#define RCC_IOPENR (*(volatile unsigned int *)0x40021034)


// ----------------------------------------------------
// SysTick Registers
// ----------------------------------------------------

#define STK_CSR (*(volatile unsigned int *)0xE000E010)
#define STK_RVR (*(volatile unsigned int *)0xE000E014)
#define STK_CVR (*(volatile unsigned int *)0xE000E018)


int game_on = 1;
int player_A = 1;
int player_B = 2;

int row;
int column;
int position;
int token;

int current_player = 1;


// ----------------------------------------------------
// Boards
// ----------------------------------------------------

int portA_board[9] = {
    9, 10, 15,
    6, 7, 8,
    5, 12, 11
};

int portB_board[9] = {
    1, 7, 6,
    8, 2, 0,
    4, 5, 9
};

int player_board[9] = {0};


// ----------------------------------------------------
// Switch Actions
// ----------------------------------------------------

int get_switch(void)
{
    // Switch 1 - PA4
    if (GPIOA_IDR & (1 << 4))
    {
        return 0;
    }

    // Switch 2 - PA1
    if (GPIOA_IDR & (1 << 1))
    {
        return 1;
    }

    // Switch 3 - PA0
    if (GPIOA_IDR & (1 << 0))
    {
        return 2;
    }

    // No switch pressed
    return -1;
}


// ----------------------------------------------------
// LED Functions
// ----------------------------------------------------

void turnA(int p)
{
    GPIOA_MODER &= ~(3 << (p * 2));
    GPIOA_MODER |= (1 << (p * 2));

    GPIOA_ODR |= (1 << p);
}


void turnB(int p)
{
    GPIOB_MODER &= ~(3 << (p * 2));
    GPIOB_MODER |= (1 << (p * 2));

    GPIOB_ODR |= (1 << p);
}


// ----------------------------------------------------
// Turn All LEDs Off
// ----------------------------------------------------

void leds_off(void)
{
    for (int i = 0; i < 9; i++)
    {
        // Turn off Player A LEDs
        GPIOA_ODR &= ~(1 << portA_board[i]);

        // Turn off Player B LEDs
        GPIOB_ODR &= ~(1 << portB_board[i]);
    }
}


// ----------------------------------------------------
// Reset Game
// ----------------------------------------------------

void reset(void)
{
    // Turn all LEDs off FIRST
    leds_off();

    // Clear all positions on the player board
    for (int i = 0; i < 9; i++)
    {
        player_board[i] = 0;
    }

    // Player A starts the new game
    current_player = player_A;
}


// ----------------------------------------------------
// Winner Check
// ----------------------------------------------------

int winner_check(void)
{
    // Player A
    if (
        (player_board[0] == player_A && player_board[1] == player_A && player_board[2] == player_A) ||
        (player_board[3] == player_A && player_board[4] == player_A && player_board[5] == player_A) ||
        (player_board[6] == player_A && player_board[7] == player_A && player_board[8] == player_A) ||

        (player_board[0] == player_A && player_board[3] == player_A && player_board[6] == player_A) ||
        (player_board[1] == player_A && player_board[4] == player_A && player_board[7] == player_A) ||
        (player_board[2] == player_A && player_board[5] == player_A && player_board[8] == player_A) ||

        (player_board[0] == player_A && player_board[4] == player_A && player_board[8] == player_A) ||
        (player_board[2] == player_A && player_board[4] == player_A && player_board[6] == player_A)
       )
    {
        return 1;
    }


    // Player B
    if (
        (player_board[0] == player_B && player_board[1] == player_B && player_board[2] == player_B) ||
        (player_board[3] == player_B && player_board[4] == player_B && player_board[5] == player_B) ||
        (player_board[6] == player_B && player_board[7] == player_B && player_board[8] == player_B) ||

        (player_board[0] == player_B && player_board[3] == player_B && player_board[6] == player_B) ||
        (player_board[1] == player_B && player_board[4] == player_B && player_board[7] == player_B) ||
        (player_board[2] == player_B && player_board[5] == player_B && player_board[8] == player_B) ||

        (player_board[0] == player_B && player_board[4] == player_B && player_board[8] == player_B) ||
        (player_board[2] == player_B && player_board[4] == player_B && player_board[6] == player_B)
       )
    {
        return 2;
    }

    // No winner
    return 0;
}


// ----------------------------------------------------
// 25 ms SysTick Delay
// ----------------------------------------------------

void delay_25ms(void)
{
    // 25 ms at 16 MHz
    STK_RVR = 400000 - 1;

    // Clear current value
    STK_CVR = 0;

    // CLKSOURCE = HCLK
    // ENABLE = 1
    STK_CSR = (1 << 2) | (1 << 0);

    // Wait for COUNTFLAG
    while ((STK_CSR & (1 << 16)) == 0)
    {
    }

    // Stop SysTick
    STK_CSR = 0;
}


// ----------------------------------------------------
// 2 Second Delay
// ----------------------------------------------------

void delay_2s(void)
{
    // 80 x 25 ms = 2000 ms = 2 seconds
    for (int i = 0; i < 80; i++)
    {
        delay_25ms();
    }
}


// ----------------------------------------------------
// Wait For Switch Release
// ----------------------------------------------------

void wait_for_release(void)
{
    while (get_switch() != -1)
    {
    }

    // Debounce after release
    delay_25ms();
}


// ----------------------------------------------------
// Main
// ----------------------------------------------------

int main(void)
{
    // Enable GPIOA
    RCC_IOPENR |= (1 << 0);

    // Enable GPIOB
    RCC_IOPENR |= (1 << 1);


    // ------------------------------------------------
    // Configure PA4, PA1, PA0 as switch inputs
    // ------------------------------------------------

    // PA4 input
    GPIOA_MODER &= ~(3 << (4 * 2));

    // PA1 input
    GPIOA_MODER &= ~(3 << (1 * 2));

    // PA0 input
    GPIOA_MODER &= ~(3 << (0 * 2));


    // ------------------------------------------------
    // Enable pull-downs on PA4, PA1, PA0
    // ------------------------------------------------

    // PA4 pull-down
    GPIOA_PUPDR &= ~(3 << (4 * 2));
    GPIOA_PUPDR |=  (2 << (4 * 2));

    // PA1 pull-down
    GPIOA_PUPDR &= ~(3 << (1 * 2));
    GPIOA_PUPDR |=  (2 << (1 * 2));

    // PA0 pull-down
    GPIOA_PUPDR &= ~(3 << (0 * 2));
    GPIOA_PUPDR |=  (2 << (0 * 2));


    // Start with an empty board
    reset();


    while (game_on)
    {
        // ====================================================
        // CYCLE 1: CHOOSE ROW
        //
        // Switch 1 = Row 0
        // Switch 2 = Row 1
        // Switch 3 = Row 2
        // ====================================================

        row = -1;

        while (row == -1)
        {
            row = get_switch();
        }

        wait_for_release();


        // ====================================================
        // CYCLE 2: CHOOSE COLUMN
        //
        // Switch 1 = Column 0
        // Switch 2 = Column 1
        // Switch 3 = Column 2
        // ====================================================

        column = -1;

        while (column == -1)
        {
            column = get_switch();
        }

        wait_for_release();


        // Convert selected row and column into board position
        position = row * 3 + column;


        // ====================================================
        // CYCLE 3: CHOOSE / PLACE TOKEN
        //
        // Switch 1 = Token choice 0
        // Switch 2 = Token choice 1
        // Switch 3 = Token choice 2
        // ====================================================

        token = -1;

        while (token == -1)
        {
            token = get_switch();
        }

        wait_for_release();


        // ----------------------------------------------------
        // Place Token
        // ----------------------------------------------------

        if (player_board[position] == 0)
        {
            player_board[position] = current_player;


            // Change players
            if (current_player == player_A)
            {
                current_player = player_B;
            }
            else
            {
                current_player = player_A;
            }
        }


        // ----------------------------------------------------
        // Update LEDs
        // ----------------------------------------------------
        // Update BEFORE checking winner/draw so final move
        // turns on before the game resets.
        // ----------------------------------------------------

        for (int i = 0; i < 9; i++)
        {
            if (player_board[i] == player_A)
            {
                turnA(portA_board[i]);
            }
            else if (player_board[i] == player_B)
            {
                turnB(portB_board[i]);
            }
        }


        // ----------------------------------------------------
        // Check for Winner
        // ----------------------------------------------------

        int result = winner_check();

        if (result == 1)
        {
            // Player A wins

            // Keep winning board visible for 2 seconds
            delay_2s();

            // Start new game
            reset();
        }
        else if (result == 2)
        {
            // Player B wins

            // Keep winning board visible for 2 seconds
            delay_2s();

            // Start new game
            reset();
        }
        else
        {
            // ------------------------------------------------
            // Check if board is full
            // ------------------------------------------------

            int full = 1;

            for (int i = 0; i < 9; i++)
            {
                if (player_board[i] == 0)
                {
                    full = 0;
                }
            }

            if (full == 1)
            {
                // Draw

                // Keep completed board visible for 2 seconds
                delay_2s();

                // Start new game
                reset();
            }
        }
    }


    return 0;
}
