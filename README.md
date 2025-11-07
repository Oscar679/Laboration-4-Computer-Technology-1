## Report for laboration 4 - Computer Technology 1

  - **My name and LNU credentials:** Oscar Ekberg, oe222ia
  - **Examiner of this report**: Daniel Lundberg

## Task 1c
**Turning on & off two LEDs**

**Link to video demonstration**
[Task 1c](https://drive.google.com/file/d/16WROyDU1ag1llOqtm1FVzAO7rwPqU0u4/view?usp=sharing)

# Source code
```assembly
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#define LED_PIN_1 0
#define LED_PIN_2 6
#define BUTTON_1 1
#define BUTTON_2 2

// Hardware registers
#define SIO_BASE 0xD0000000u                                       // Start of GPIO section in the memory
#define SIO_GPIO_IN ((volatile uint32_t *)(SIO_BASE + 0x004))      // Current input
#define SIO_GPIO_OUT_SET ((volatile uint32_t *)(SIO_BASE + 0x014)) // Setting this bit to "1" sets that GPIO to HIGH
#define SIO_GPIO_OUT_CLR ((volatile uint32_t *)(SIO_BASE + 0x018)) // Setting this bit to "1" sets that GPIO to LOW

int main(void)
{
    stdio_init_all();

    gpio_init(LED_PIN_1);
    gpio_set_function(LED_PIN_1, GPIO_FUNC_SIO);
    gpio_set_dir(LED_PIN_1, GPIO_OUT);

    gpio_init(LED_PIN_2);
    gpio_set_function(LED_PIN_2, GPIO_FUNC_SIO);
    gpio_set_dir(LED_PIN_2, GPIO_OUT);

    gpio_init(BUTTON_1);
    gpio_init(BUTTON_2);

    gpio_set_dir(BUTTON_1, GPIO_IN);
    gpio_set_dir(BUTTON_2, GPIO_IN);

    // Default is HIGH when NOT pressed
    gpio_pull_up(BUTTON_1);
    gpio_pull_up(BUTTON_2);

    while (1)
    {
        // Reads the value of all the 32 bits (pins)
        uint32_t pins = *SIO_GPIO_IN;
        // Checks if the first button is equal to LOW (pressed). Shifts to check the value of the 3rd bit (GP3)
        if ((pins & (1u << BUTTON_1)) == 0)
        {
            // Turns LED ON (GP0), shifts the program to alter the 0th bit and setting it to HIGH
            *SIO_GPIO_OUT_SET = (1u << LED_PIN_1);

            *SIO_GPIO_OUT_SET = (1u << LED_PIN_2);
        }

        // Checks if the first button is equal to LOW (pressed). Shifts to check the value of the 4th bit (GP4)
        if ((pins & (1u << BUTTON_2)) == 0)
        {
            // Turns LED OFF, shifts the program to alter the 0th bit and clearing the bit (setting it to low)
            *SIO_GPIO_OUT_CLR = (1u << LED_PIN_1);

            *SIO_GPIO_OUT_CLR = (1u << LED_PIN_2);
        }

        sleep_ms(100); // Small delay
    }
    return 0;
}

```

## Task 2 - Binary counter
**One button for increasing counter, one button for decreasing counter**

**Note - Since I had already turned in the buttons I was given by my lab teacher I had to simulate the button functionality by manually connecting the wires. Hence the "buttons" were pressed several times and it looks quite strange.**

**Link to video demonstration**
[Task 2](https://drive.google.com/file/d/1IatcQ3Dx6XHZcwLqbpBWX-2vXySqFk1A/view?usp=sharing)

# Source code
```assembly
#include "pico/stdlib.h"

// Counter is altered and modified within an interrupt service routine, hence 'volatile'
volatile uint8_t counter = 0;

#define LED_PIN_1 1
#define LED_PIN_2 2
#define LED_PIN_3 3
#define LED_PIN_4 4

#define BUTTON_1 5
#define BUTTON_2 6

int pattern_array[] = {
    0b0000,
    0b0001,
    0b0010,
    0b0011,
    0b0100,
    0b0101,
    0b0110,
    0b0111,
    0b1000,
    0b1001,
    0b1010,
    0b1011,
    0b1100,
    0b1101,
    0b1110,
    0b1111};

// Updates LEDs
void updateLeds(int pattern)
{
    // The pattern (current index of pattern_array) is shifted so that the value, 0 or 1, is at the right most bit. The value is then sent as a parameter to gpio_put()
    gpio_put(LED_PIN_4, (pattern >> 3) & 0x1); // 0x1 ensures that only the right most bit is handled
    gpio_put(LED_PIN_3, (pattern >> 2) & 0x1);
    gpio_put(LED_PIN_2, (pattern >> 1) & 0x1);
    gpio_put(LED_PIN_1, pattern & 0x1);
}

// Interrupt Service Routine for button
void button_isr(uint gpio, uint events)
{
    if (gpio == BUTTON_1)
    {
        counter = (counter + 1) & 0xF; // Ensures that the counter increases by 1, and is NOT greater than 15
    }
    else if (gpio == BUTTON_2)
    {
        counter = (counter - 1) & 0xF; // Ensures that the counter decreases by 1, and is NOT greater than 15
    }
    updateLeds(pattern_array[counter]); // Calls method with current binary number as parameter
}

int main(void)
{
    stdio_init_all();

    // Initiate LEDs
    gpio_init(LED_PIN_1);
    gpio_init(LED_PIN_2);
    gpio_init(LED_PIN_3);
    gpio_init(LED_PIN_4);

    gpio_set_dir(LED_PIN_1, GPIO_OUT);
    gpio_set_dir(LED_PIN_2, GPIO_OUT);
    gpio_set_dir(LED_PIN_3, GPIO_OUT);
    gpio_set_dir(LED_PIN_4, GPIO_OUT);

    // Initiate buttons
    gpio_init(BUTTON_1);
    gpio_init(BUTTON_2);

    gpio_set_dir(BUTTON_1, GPIO_IN);
    gpio_set_dir(BUTTON_2, GPIO_IN);

    gpio_pull_up(BUTTON_1);
    gpio_pull_up(BUTTON_2);

    gpio_set_irq_enabled_with_callback(BUTTON_1, GPIO_IRQ_EDGE_FALL, true, &button_isr);
    gpio_set_irq_enabled(BUTTON_2, GPIO_IRQ_EDGE_FALL, true);

    // Keeps the program alive
    while (1)
    {
    }
    return 0;
}

```

## Task 3 - Automatic binary counter with reset button

**Link to video demonstration**
[Task 3](https://drive.google.com/file/d/1jTDifWT3uNiQIdZgJ47wzesrpHNy8UkK/view?usp=sharing)

# Source code
```assembly
#include "pico/stdlib.h"

uint8_t counter = 0;

#define LED_PIN_1 1
#define LED_PIN_2 2
#define LED_PIN_3 3
#define LED_PIN_4 4

#define BUTTON_1 0

int pattern_array[] = {
    0b0000,
    0b0001,
    0b0010,
    0b0011,
    0b0100,
    0b0101,
    0b0110,
    0b0111,
    0b1000,
    0b1001,
    0b1010,
    0b1011,
    0b1100,
    0b1101,
    0b1110,
    0b1111};

void resetLeds()
{
    gpio_put(LED_PIN_1, 0);
    gpio_put(LED_PIN_2, 0);
    gpio_put(LED_PIN_3, 0);
    gpio_put(LED_PIN_4, 0);
}

// The pattern (current index of pattern_array) is shifted so that the value, 0 or 1, is at the right most bit. The value is then sent as a parameter to gpio_put()
void setLeds(int pattern)
{
    gpio_put(LED_PIN_1, pattern & 1);
    gpio_put(LED_PIN_2, (pattern >> 1) & 1);
    gpio_put(LED_PIN_3, (pattern >> 2) & 1);
    gpio_put(LED_PIN_4, (pattern >> 3) & 1);
}

int64_t timer_isr()
{
    if (counter >= 15)
    {
        return 0;
    }
    counter++;
    resetLeds();
    setLeds(pattern_array[counter]);

    return 100 * 10000; // 1 second
}

void button_isr(uint gpio, uint32_t events)
{
    if (gpio == BUTTON_1)
    {
        counter = 0;
        resetLeds();
    }
}

int main(void)
{

    stdio_init_all();

    // Initiate LEDs
    gpio_init(LED_PIN_1);
    gpio_init(LED_PIN_2);
    gpio_init(LED_PIN_3);
    gpio_init(LED_PIN_4);

    gpio_set_dir(LED_PIN_1, GPIO_OUT);
    gpio_set_dir(LED_PIN_2, GPIO_OUT);
    gpio_set_dir(LED_PIN_3, GPIO_OUT);
    gpio_set_dir(LED_PIN_4, GPIO_OUT);
    resetLeds();

    // Initiate buttons
    gpio_init(BUTTON_1);
    gpio_set_dir(BUTTON_1, GPIO_IN);

    gpio_pull_up(BUTTON_1);

    // Configure interrupt for the button
    gpio_set_irq_enabled_with_callback(BUTTON_1, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // Configure timer interrupt
    add_alarm_in_us(100 * 10000, timer_isr, NULL, true);

    // Keeps the program alive
    while (1)
    {
        tight_loop_contents();
    }
    return 0;
}

```
