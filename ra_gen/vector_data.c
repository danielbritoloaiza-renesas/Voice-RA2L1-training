/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_uart_rxi_isr, /* SCI3 RXI (Receive data full) */
            [1] = sci_uart_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [2] = sci_uart_tei_isr, /* SCI3 TEI (Transmit end) */
            [3] = sci_uart_eri_isr, /* SCI3 ERI (Receive error) */
            [4] = adc_scan_end_isr, /* ADC0 SCAN END (End of A/D scanning operation) */
            [5] = spi_txi_isr, /* SPI0 TXI (Transmit buffer empty) */
            [6] = adc_window_compare_isr, /* ADC0 WINDOW A (Window A Compare match interrupt) */
            [7] = adc_window_compare_isr, /* ADC0 WINDOW B (Window B Compare match interrupt) */
            [8] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [9] = user_def_adc120_wcmpum, /* ADC0 COMPARE MISMATCH (Compare mismatch) */
            [10] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [11] = spi_eri_isr, /* SPI0 ERI (Error) */
            [12] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [14] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [15] = user_def_isr_snz_req, /* LPM SNOOZE REQUEST (Snooze entry) */
            [16] = user_def_adc120_wcmpm, /* ADC0 COMPARE MATCH (Compare match) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_SCI3_RXI,GROUP0), /* SCI3 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TXI,GROUP1), /* SCI3 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TEI,GROUP2), /* SCI3 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI3_ERI,GROUP3), /* SCI3 ERI (Receive error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_ADC0_SCAN_END,GROUP4), /* ADC0 SCAN END (End of A/D scanning operation) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TXI,GROUP5), /* SPI0 TXI (Transmit buffer empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_ADC0_WINDOW_A,GROUP6), /* ADC0 WINDOW A (Window A Compare match interrupt) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_ADC0_WINDOW_B,GROUP7), /* ADC0 WINDOW B (Window B Compare match interrupt) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP0), /* SPI0 RXI (Receive buffer full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_ADC0_COMPARE_MISMATCH,GROUP1), /* ADC0 COMPARE MISMATCH (Compare mismatch) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP2), /* SPI0 TEI (Transmission complete event) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP3), /* SPI0 ERI (Error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP4), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,GROUP6), /* FCU FRDYI (Flash ready interrupt) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_LPM_SNOOZE_REQUEST,GROUP7), /* LPM SNOOZE REQUEST (Snooze entry) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_ADC0_COMPARE_MATCH,GROUP0), /* ADC0 COMPARE MATCH (Compare match) */
        };
        #endif
        #endif
