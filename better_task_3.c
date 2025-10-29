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

    return 100 * 10000;
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
}
