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

int main()
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