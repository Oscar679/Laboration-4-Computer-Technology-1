#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

const int LED_PIN = 0;
const int BUTTON_1 = 1;
const int BUTTON_2 = 2;

int main(void)
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_1);
    gpio_init(BUTTON_2);
    gpio_set_dir(BUTTON_1, GPIO_IN);
    gpio_set_dir(BUTTON_2, GPIO_IN);

    gpio_pull_up(BUTTON_1);
    gpio_pull_up(BUTTON_2);

    while (1)
    {
        if (gpio_get(BUTTON_1) == 0)
        {
            gpio_put(LED_PIN, 1);
            sleep_ms(100);
        }

        if (gpio_get(BUTTON_2) == 0)
        {
            gpio_put(LED_PIN, 0);
            sleep_ms(100);
        }
    }
    return 0;

}
