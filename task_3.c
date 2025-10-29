#include "pico/stdlib.h"

uint8_t counter = 0;

#define LED_PIN_1 1
#define LED_PIN_2 2
#define LED_PIN_3 3
#define LED_PIN_4 4

#define BUTTON_1 0

void resetLeds()
{
    gpio_put(LED_PIN_1, 0);
    gpio_put(LED_PIN_2, 0);
    gpio_put(LED_PIN_3, 0);
    gpio_put(LED_PIN_4, 0);
}

int64_t timer_isr()
{
    if (counter >= 15)
    {
        return 0;
    }
    counter++;

    resetLeds();
    switch (counter)
    {
    case 1:
        gpio_put(LED_PIN_1, 1);
        break;

    case 2:
        gpio_put(LED_PIN_2, 1);
        break;

    case 3:
        gpio_put(LED_PIN_1, 1);
        gpio_put(LED_PIN_2, 1);
        break;

    case 4:
        gpio_put(LED_PIN_3, 1);
        break;

    case 5:
        gpio_put(LED_PIN_1, 1);
        gpio_put(LED_PIN_3, 1);
        break;

    case 6:
        gpio_put(LED_PIN_2, 1);
        gpio_put(LED_PIN_3, 1);
        break;

    case 7:
        gpio_put(LED_PIN_1, 1);
        gpio_put(LED_PIN_2, 1);
        gpio_put(LED_PIN_3, 1);
        break;

    case 8:
        gpio_put(LED_PIN_4, 1);
        break;

    case 9:
        gpio_put(LED_PIN_1, 1);
        gpio_put(LED_PIN_4, 1);
        break;

    case 10:
        gpio_put(LED_PIN_2, 1);
        gpio_put(LED_PIN_4, 1);
        break;

    case 11:
        gpio_put(LED_PIN_1, 1);
        gpio_put(LED_PIN_2, 1);
        gpio_put(LED_PIN_4, 1);
        break;

    case 12:
        gpio_put(LED_PIN_3, 1);
        gpio_put(LED_PIN_4, 1);
        break;

    case 13:
        gpio_put(LED_PIN_1, 1);
        gpio_put(LED_PIN_3, 1);
        gpio_put(LED_PIN_4, 1);
        break;

    case 14:
        gpio_put(LED_PIN_2, 1);
        gpio_put(LED_PIN_3, 1);
        gpio_put(LED_PIN_4, 1);
        break;

    case 15:
        gpio_put(LED_PIN_1, 1);
        gpio_put(LED_PIN_2, 1);
        gpio_put(LED_PIN_3, 1);
        gpio_put(LED_PIN_4, 1);
        break;
    }
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
    irq_set_enabled(IO_IRQ_BANK0, true);

    // Configure timer interrupt
    add_alarm_in_us(100 * 10000, timer_isr, NULL, true);

    // Keeps the program alive
    while (1)
    {
        tight_loop_contents();
    }
}