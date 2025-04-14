#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"



int main()
{
    stdio_init_all();
    adc_init(); // Initialize the ADC

    adc_gpio_init(26); // Initialize GPIO 26 for ADC input
    adc_gpio_init(27); // Initialize GPIO 27 for ADC input

    while (true) {
        adc_select_input(0); //
        uint adc_y_raw = adc_read();
        adc_select_input(1); // entrada adc 1
        uint adc_x_raw = adc_read();

        const uint bar_width = 40;
        const uint adc_max = (1 << 12) -1;
        uint bar_x_pos = (adc_x_raw * bar_width) / adc_max;
        uint bar_y_pos = (adc_y_raw * bar_width) / adc_max;
        printf("\rX: [");       
        for(uint i = 0; i < bar_x_pos; i++) {
            putchar( i == bar_x_pos ? 'o' : ' ');
            printf("] Y: [");
        }
        for(uint i = 0; i < bar_y_pos; i++) {
            putchar( i == bar_y_pos ? 'o' : ' ');
            printf("] ");
            sleep_ms(50); 
        }
    }
}
