#ifndef __LPM_ADC_H
#define __LPM_ADC_H

#include <stdbool.h>

int lpm_adc_open(void);
int lpm_adc_close(void);
int lpm_adc_enter(void);
bool lpm_adc_is_sound_detect(void);
void lpm_adc_reset_sound_detect(void);

#endif
