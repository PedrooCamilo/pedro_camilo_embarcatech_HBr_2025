#include "hal_led.h"
#include "led_embutido.h"
#include "pico/stdlib.h"

static int estado_atual = 0;

void hal_led_init(void) {
    stdio_init_all();
    led_embutido_init();
}

void hal_led_toggle(void) {
    estado_atual = !estado_atual;
    led_embutido_set(estado_atual);
}
