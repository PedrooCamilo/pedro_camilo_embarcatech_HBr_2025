#include "temp_convert.h"

//VALORES OBTIDOS DA DOCUMENTAÇÃO DO RP2040
const float ADC = 3.3f;
const float ADC_RANGE = (1 << 12); 
const float CONVERSION_ADC = ADC / (ADC_RANGE - 1); 
const float VOLTAGE_27C = 0.706f;
const float SLOPE1 = 0.001721f;

float adc_to_celsius(uint16_t ADC_VALUE) {
    if (ADC_VALUE > 4095) {
        ADC_VALUE = 4095;
    }
    float ADC_VOLTAGE = (float)ADC_VALUE * CONVERSION_ADC;
    float TEMP_CELSIUS = 27.0f - (ADC_VOLTAGE - VOLTAGE_27C) / SLOPE1;

    return TEMP_CELSIUS;
}