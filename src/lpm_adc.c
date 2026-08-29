/*
 * lpm_adc.c
 *
 *  Created on: 2022/10/6
 *      Author: mingwu
 */

#include <stdbool.h>
#include "hal_data.h"
#include "lpm_adc.h"


bool g_bSoundDetected = false;


int lpm_adc_open(void)
{
    fsp_err_t err = FSP_SUCCESS;

    g_bSoundDetected = false;

    while (true)
    {
        /** ADC */
        err = R_ADC_Open(&g_adc_lpm_ctrl, &g_adc_lpm_cfg);
        if( FSP_SUCCESS != err )
            break;

        err = R_ADC_ScanCfg(&g_adc_lpm_ctrl, &g_adc_lpm_channel_cfg);
        if( FSP_SUCCESS != err )
            break;

        R_BSP_IrqCfgEnable(ADC0_COMPARE_MATCH_IRQn, 2, &g_adc_lpm_ctrl);       // Set an interrupt priority, for ADC120_WCMPM interrupt
        R_BSP_IrqCfgEnable(ADC0_COMPARE_MISMATCH_IRQn, 2, &g_adc_lpm_ctrl);    // Set an interrupt priority, for ADC12_WCMPUM interrupt

        /** ELC */
		err = R_ELC_Open(&g_elc_ctrl, &g_elc_cfg);
		if( FSP_SUCCESS != err )
			break;

	    err = R_ELC_LinkSet(&g_elc_ctrl, ELC_PERIPHERAL_ADC0, ELC_EVENT_LPM_SNOOZE_REQUEST);  /* LPM SNOOZE REQUEST (Snooze entry) */
        if( FSP_SUCCESS != err )
			break;

        err = R_ELC_Enable(&g_elc_ctrl);
        if( FSP_SUCCESS != err )
            break;

        /** AGT */
        err = R_AGT_Open(&g_agt1_lpm_ctrl, &g_agt1_lpm_cfg);
        if( FSP_SUCCESS != err )
            break;

        err = R_LPM_Open(&g_lpm0_ctrl, &g_lpm0_cfg);
        if( FSP_SUCCESS != err )
            break;

        err = R_AGT_Start(&g_agt1_lpm_ctrl);
        if( FSP_SUCCESS != err )
            break;

        err = R_ADC_ScanStart(&g_adc_lpm_ctrl);
        if( FSP_SUCCESS != err )
            break;

        break;
    }

#ifdef DEBUG
    if( FSP_SUCCESS != err )
        __BKPT(1);
#endif

    return err;
}


int lpm_adc_close(void)
{
    R_ADC_ScanStop(&g_adc_lpm_ctrl);
    R_AGT_Stop(&g_agt1_lpm_ctrl);
    R_LPM_Close(&g_lpm0_ctrl);
    R_AGT_Close(&g_agt1_lpm_ctrl);
    R_ELC_Disable(&g_elc_ctrl);
    R_ELC_Close(&g_elc_ctrl);
    R_ADC_Close(&g_adc_lpm_ctrl);

    return FSP_SUCCESS;
}

int lpm_adc_enter(void)
{
    fsp_err_t err = R_LPM_LowPowerModeEnter(&g_lpm0_ctrl);

#ifdef DEBUG
    if( FSP_SUCCESS != err )
        __BKPT(1);
#endif

    return err;
}


bool lpm_adc_is_sound_detect(void)
{
    return g_bSoundDetected;
}


void lpm_adc_reset_sound_detect(void)
{
    g_bSoundDetected = false;
}


void user_def_isr_snz_req(void)
{

}


void user_def_adc120_wcmpm(void)
{
    R_BSP_IrqStatusClear(ADC0_COMPARE_MATCH_IRQn);

    // Window A/B composite conditions are met.
    g_bSoundDetected = true;
}


void user_def_adc120_wcmpum(void)
{
    R_BSP_IrqStatusClear(ADC0_COMPARE_MISMATCH_IRQn);

    // Window A/B composite conditions are not met.
}


