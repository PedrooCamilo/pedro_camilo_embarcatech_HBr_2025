#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h" 

#define BUTTON_A 5
#define BUTTON_B 6

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDR 0x3C

volatile int c = 0;
volatile int clicks = 0;
volatile bool count = false;
volatile bool start = false;
volatile bool update = false;

// Buffer para o display OLED
uint8_t ssd[ssd1306_buffer_length];

// Área de renderização para o display
struct render_area frame_area;

// Funções
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && !count) {
        c = 9;
        clicks = 0;
        start = true;  // apenas sinaliza
    } else if (gpio == BUTTON_B && count) {
        clicks++;
        update = true;  // apenas sinaliza
    }
}

void ssd1306_clear() {
    memset(ssd, 0, ssd1306_buffer_length); // limpa o buffer manualmente
    render_on_display(ssd, &frame_area);
}

void ssd1306_show() {
    render_on_display(ssd, &frame_area);
}

void update_display() {
    char buffer[32];
    ssd1306_clear();
    
    sprintf(buffer, "Contador: %d", c);
    // printf("%s\n", buffer);
    ssd1306_draw_string(ssd, 0, 0, buffer);
    
    sprintf(buffer, "Clicks: %d", clicks);
    // printf("%s\n", buffer);
    ssd1306_draw_string(ssd, 0, 16, buffer);
    
    ssd1306_show();
}

void start_countdown() {
    while (count && c > 0) {
        update_display();
        sleep_ms(1000);
        c--;
    }
    count = false;
    update_display();
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Espera 2 segundos para estabilizar a USB
    printf("Sistema iniciado...\n");

    // Inicializa botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    // Inicializa I2C
    i2c_init(I2C_PORT, ssd1306_i2c_clock * 1000); // 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa display OLED
    ssd1306_init(I2C_PORT, OLED_ADDR, 128, 64);
    
    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    frame_area = (struct render_area){
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);
    ssd1306_clear();
    // ssd1306_show();
    
    while (1) {
        if (start) {
            start = false;
            count = true;
            start_countdown();
        }
        if (update) {
            update = false;
            update_display();
        }
        tight_loop_contents();  // ou outras tarefas
    }
    
    return 0;
}

