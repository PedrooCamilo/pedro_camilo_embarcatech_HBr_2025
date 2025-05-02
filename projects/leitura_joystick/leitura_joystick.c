#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;


int main()
{
    stdio_init_all();
    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    adc_init(); // Initialize the ADC
    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    adc_gpio_init(26); // Initialize GPIO 26 for ADC input
    adc_gpio_init(27); // Initialize GPIO 27 for ADC input

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

// Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)
// /**
    char *text[] = {
        "  Bem-vindos!   ",
        "  Embarcatech   "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);

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
