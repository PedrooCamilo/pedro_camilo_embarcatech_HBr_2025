#include "unity.h"
#include "temp_convert.h"
#include "pico/stdlib.h"

const float VREF_ADC = 3.3f;
const float MAX_RANGE_ADC = 4095.0f; // (1 << 12) - 1
const float CONVERSION = VREF_ADC / MAX_RANGE_ADC;
const float VOLTAGE_REF = 0.706f;
const float SLOPE = 0.001721f;

void setUp(void) {}
void tearDown(void) {}

// Teste para valores ADC próximos ao ponto de referência (0.706V)
void test_adc_to_celsius_reference(void)
{
    uint16_t TEST_ADC = 876;
    float VOLTAGE = (float)TEST_ADC * CONVERSION;
    float TEMPERATURE = 27.0f - (VOLTAGE - VOLTAGE_REF) / SLOPE;
    float DELT = 0.001f; 
    float TEMP_GOT = adc_to_celsius(TEST_ADC);
    printf("Temperatura convertida (ADC = %d): %.2f °C\n", TEST_ADC, TEMP_GOT);
    // Verifica se a temperatura convertida está dentro da variação esperada
    TEST_ASSERT_FLOAT_WITHIN(DELT, TEMPERATURE, TEMP_GOT);
}

// Teste para 20C
void test_adc_to_celsius_30c(void)
{
    uint16_t TEST_ADC = 869; 
    float VOLTAGE = (float)TEST_ADC * CONVERSION;
    float TEMPERATURE = 27.0f - (VOLTAGE - VOLTAGE_REF) / SLOPE;
    float DELT = 0.01f; 
    float TEMP_GOT = adc_to_celsius(TEST_ADC);
    printf("Temperatura convertida (ADC = %d): %.2f °C\n", TEST_ADC, TEMP_GOT);
    TEST_ASSERT_FLOAT_WITHIN(DELT, TEMPERATURE, TEMP_GOT);
}

// Teste para -30
void test_adc_to_celsius_30negative(void)
{
    uint16_t TEST_ADC = 1000;
    float VOLTAGE = (float)TEST_ADC * CONVERSION;
    float TEMPERATURE = 27.0f - (VOLTAGE - VOLTAGE_REF) / SLOPE;
    float DELT = 0.01f;                                                                         
    float TEMP_GOT = adc_to_celsius(TEST_ADC);
    printf("Temperatura convertida (ADC = %d): %.2f °C\n", TEST_ADC, TEMP_GOT);
    TEST_ASSERT_FLOAT_WITHIN(DELT, TEMPERATURE, TEMP_GOT);
}

// Teste para valor alto aleatório
void test_adc_to_celsius_(void)
{
    uint16_t TEST_ADC = 840;
    float VOLTAGE = (float)TEST_ADC * CONVERSION;
    float TEMPERATURE = 27.0f - (VOLTAGE - VOLTAGE_REF) / SLOPE; 
    float DELT = 0.01f;                                                                              
    float TEMP_GOT = adc_to_celsius(TEST_ADC);
    printf("Temperatura convertida (ADC = %d): %.2f °C\n", TEST_ADC, TEMP_GOT);
    TEST_ASSERT_FLOAT_WITHIN(DELT, TEMPERATURE, TEMP_GOT);
}

int main(void)
{
    stdio_init_all();
    sleep_ms(6000); 

    // Inicializa o Unity
    UNITY_BEGIN();
    printf("iniciando os testes\n");
    sleep_ms(2500);

    printf("testando a conversão de referência\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_reference);
    sleep_ms(2500);

    printf("testando conversão para 30C\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_30c);
    sleep_ms(2500);

    printf("testando conversão para -30C\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_30negative);
    sleep_ms(2500);

    printf("testando conversão para valor aleatório\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_);
    sleep_ms(2500);

    printf("resultados:\n");
    return UNITY_END();
}