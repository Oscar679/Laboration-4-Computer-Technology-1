#include "pico/stdlib.h"

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

void updateLeds(int pattern)
{
    gpio_put(LED_PIN_4, (pattern >> 3) & 0x1);
    gpio_put(LED_PIN_3, (pattern >> 2) & 0x1);
    gpio_put(LED_PIN_2, (pattern >> 1) & 0x1);
    gpio_put(LED_PIN_1, pattern & 0x1);
}

void button_isr(uint gpio, uint events)
{
    if (gpio == BUTTON_1)
    {
        counter = (counter + 1) & 0xF;
    }
    else if (gpio == BUTTON_2)
    {
        counter = (counter - 1) & 0xF;
    }
    updateLeds(pattern_array[counter]);
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

    while (1)
    {
    }
}
