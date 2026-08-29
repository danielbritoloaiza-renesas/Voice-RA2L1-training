/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_dac.h"
#include "r_dac_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_flash_lp.h"
#include "r_flash_api.h"
#include "r_poeg.h"
#include "r_poeg_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_spi.h"
#include "r_adc.h"
#include "r_adc_api.h"
#include "r_agt.h"
#include "r_timer_api.h"
#include "r_lpm.h"
#include "r_lpm_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_playback;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer_playback_ctrl;
extern const timer_cfg_t g_timer_playback_cfg;

#ifndef g_timer_playback_callback
void g_timer_playback_callback(timer_callback_args_t *p_args);
#endif
/** DAC on DAC Instance. */
extern const dac_instance_t g_dac_playback;

/** Access the DAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dac_instance_ctrl_t g_dac_playback_ctrl;
extern const dac_cfg_t g_dac_playback_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart_ds;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart_ds_ctrl;
extern const uart_cfg_t g_uart_ds_cfg;
extern const sci_uart_extended_cfg_t g_uart_ds_cfg_extend;

#ifndef user_uart_callback
void user_uart_callback(uart_callback_args_t *p_args);
#endif
/* Flash on Flash LP Instance. */
extern const flash_instance_t g_flash0;

/** Access the Flash LP instance using these structures when calling API functions directly (::p_api is not used). */
extern flash_lp_instance_ctrl_t g_flash0_ctrl;
extern const flash_cfg_t g_flash0_cfg;

#ifndef flash0_bgo_callback
void flash0_bgo_callback(flash_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_ssl;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer_ssl_ctrl;
extern const timer_cfg_t g_timer_ssl_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** POEG Instance. */
extern const poeg_instance_t g_poeg_ws;

/** Access the POEG instance using these structures when calling API functions directly (::p_api is not used). */
extern poeg_instance_ctrl_t g_poeg_ws_ctrl;
extern const poeg_cfg_t g_poeg_ws_cfg;

#ifndef NULL
void NULL(poeg_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer1_ctrl;
extern const transfer_cfg_t g_transfer1_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer0_ctrl;
extern const transfer_cfg_t g_transfer0_cfg;
/** SPI on SPI Instance. */
extern const spi_instance_t g_spi_i2s;

/** Access the SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern spi_instance_ctrl_t g_spi_i2s_ctrl;
extern const spi_cfg_t g_spi_i2s_cfg;

/** Callback used by SPI Instance. */
#ifndef g_spi_cb
void g_spi_cb(spi_callback_args_t *p_args);
#endif

#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer0)
    #define g_spi_i2s_P_TRANSFER_TX (NULL)
#else
#define g_spi_i2s_P_TRANSFER_TX (&g_transfer0)
#endif
#if (RA_NOT_DEFINED == g_transfer1)
    #define g_spi_i2s_P_TRANSFER_RX (NULL)
#else
#define g_spi_i2s_P_TRANSFER_RX (&g_transfer1)
#endif
#undef RA_NOT_DEFINED
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_ws;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer_ws_ctrl;
extern const timer_cfg_t g_timer_ws_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_sck;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer_sck_ctrl;
extern const timer_cfg_t g_timer_sck_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_adc_dtc_transfer;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_adc_dtc_transfer_ctrl;
extern const transfer_cfg_t g_adc_dtc_transfer_cfg;
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc_amic;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc_amic_ctrl;
extern const adc_cfg_t g_adc_amic_cfg;
extern const adc_channel_cfg_t g_adc_amic_channel_cfg;

#ifndef adc_dtc_end_cb
void adc_dtc_end_cb(adc_callback_args_t *p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  1
#endif
/** AGT Timer Instance */
extern const timer_instance_t g_timer_amic;

/** Access the AGT instance using these structures when calling API functions directly (::p_api is not used). */
extern agt_instance_ctrl_t g_timer_amic_ctrl;
extern const timer_cfg_t g_timer_amic_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc_lpm;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc_lpm_ctrl;
extern const adc_cfg_t g_adc_lpm_cfg;
extern const adc_channel_cfg_t g_adc_lpm_channel_cfg;

#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  2
#endif
/** lpm Instance */
extern const lpm_instance_t g_lpm0;

/** Access the LPM instance using these structures when calling API functions directly (::p_api is not used). */
extern lpm_instance_ctrl_t g_lpm0_ctrl;
extern const lpm_cfg_t g_lpm0_cfg;
/** AGT Timer Instance */
extern const timer_instance_t g_agt1_lpm;

/** Access the AGT instance using these structures when calling API functions directly (::p_api is not used). */
extern agt_instance_ctrl_t g_agt1_lpm_ctrl;
extern const timer_cfg_t g_agt1_lpm_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
