/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (16)
#endif
/* ISR prototypes */
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_eri_isr(void);
void adc_scan_end_isr(void);
void spi_txi_isr(void);
void adc_window_compare_isr(void);
void spi_rxi_isr(void);
void user_def_adc120_wcmpum(void);
void spi_tei_isr(void);
void spi_eri_isr(void);
void gpt_counter_overflow_isr(void);
void fcu_frdyi_isr(void);
void user_def_isr_snz_req(void);
void user_def_adc120_wcmpm(void);

/* Vector table allocations */
#define VECTOR_NUMBER_SCI3_RXI ((IRQn_Type) 0) /* SCI3 RXI (Receive data full) */
#define SCI3_RXI_IRQn          ((IRQn_Type) 0) /* SCI3 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 1) /* SCI3 TXI (Transmit data empty) */
#define SCI3_TXI_IRQn          ((IRQn_Type) 1) /* SCI3 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 2) /* SCI3 TEI (Transmit end) */
#define SCI3_TEI_IRQn          ((IRQn_Type) 2) /* SCI3 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI3_ERI ((IRQn_Type) 3) /* SCI3 ERI (Receive error) */
#define SCI3_ERI_IRQn          ((IRQn_Type) 3) /* SCI3 ERI (Receive error) */
#define VECTOR_NUMBER_ADC0_SCAN_END ((IRQn_Type) 4) /* ADC0 SCAN END (End of A/D scanning operation) */
#define ADC0_SCAN_END_IRQn          ((IRQn_Type) 4) /* ADC0 SCAN END (End of A/D scanning operation) */
#define VECTOR_NUMBER_SPI0_TXI ((IRQn_Type) 5) /* SPI0 TXI (Transmit buffer empty) */
#define SPI0_TXI_IRQn          ((IRQn_Type) 5) /* SPI0 TXI (Transmit buffer empty) */
#define VECTOR_NUMBER_ADC0_WINDOW_A ((IRQn_Type) 6) /* ADC0 WINDOW A (Window A Compare match interrupt) */
#define ADC0_WINDOW_A_IRQn          ((IRQn_Type) 6) /* ADC0 WINDOW A (Window A Compare match interrupt) */
#define VECTOR_NUMBER_ADC0_WINDOW_B ((IRQn_Type) 7) /* ADC0 WINDOW B (Window B Compare match interrupt) */
#define ADC0_WINDOW_B_IRQn          ((IRQn_Type) 7) /* ADC0 WINDOW B (Window B Compare match interrupt) */
#define VECTOR_NUMBER_SPI0_RXI ((IRQn_Type) 8) /* SPI0 RXI (Receive buffer full) */
#define SPI0_RXI_IRQn          ((IRQn_Type) 8) /* SPI0 RXI (Receive buffer full) */
#define VECTOR_NUMBER_ADC0_COMPARE_MISMATCH ((IRQn_Type) 9) /* ADC0 COMPARE MISMATCH (Compare mismatch) */
#define ADC0_COMPARE_MISMATCH_IRQn          ((IRQn_Type) 9) /* ADC0 COMPARE MISMATCH (Compare mismatch) */
#define VECTOR_NUMBER_SPI0_TEI ((IRQn_Type) 10) /* SPI0 TEI (Transmission complete event) */
#define SPI0_TEI_IRQn          ((IRQn_Type) 10) /* SPI0 TEI (Transmission complete event) */
#define VECTOR_NUMBER_SPI0_ERI ((IRQn_Type) 11) /* SPI0 ERI (Error) */
#define SPI0_ERI_IRQn          ((IRQn_Type) 11) /* SPI0 ERI (Error) */
#define VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW ((IRQn_Type) 12) /* GPT0 COUNTER OVERFLOW (Overflow) */
#define GPT0_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 12) /* GPT0 COUNTER OVERFLOW (Overflow) */
#define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 14) /* FCU FRDYI (Flash ready interrupt) */
#define FCU_FRDYI_IRQn          ((IRQn_Type) 14) /* FCU FRDYI (Flash ready interrupt) */
#define VECTOR_NUMBER_LPM_SNOOZE_REQUEST ((IRQn_Type) 15) /* LPM SNOOZE REQUEST (Snooze entry) */
#define LPM_SNOOZE_REQUEST_IRQn          ((IRQn_Type) 15) /* LPM SNOOZE REQUEST (Snooze entry) */
#define VECTOR_NUMBER_ADC0_COMPARE_MATCH ((IRQn_Type) 16) /* ADC0 COMPARE MATCH (Compare match) */
#define ADC0_COMPARE_MATCH_IRQn          ((IRQn_Type) 16) /* ADC0 COMPARE MATCH (Compare match) */
/* The number of entries required for the ICU vector table. */
#define BSP_ICU_VECTOR_NUM_ENTRIES (17)

#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
