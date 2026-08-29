/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

#ifndef VOICE_MAIN_H
#define VOICE_MAIN_H

/*******************************************************************************
 Include
*******************************************************************************/
#include "hal_data.h"

/*******************************************************************************
 Macro definitions
*******************************************************************************/

// Definition for support function
#define SUPPORT_SPEEX_PLAY                               // To decode Speex compressed audio and play it after getting the result.
//#define SUPPORT_UART_DUMP_RECORD                         // Transfer record data through UART(460800 bps) to PC CybSerialRecorder.
#define SUPPORT_LOW_POWER_MODE

#define NOT_SHOW_MULTI_PRONUNCIATION                     // Don't display the command string that include '^' character.


// Definition for recognition flow
#define RECOG_FLOW_NONE          0                       // Record only, no recognition.
#define RECOG_FLOW_NORMAL        1                       // We treat the first group as trigger words and the second group as command words if it exist.


#define RECOG_FLOW               RECOG_FLOW_NORMAL

#ifdef SUPPORT_UART_DUMP_RECORD
#undef  RECOG_FLOW
#define RECOG_FLOW               RECOG_FLOW_NONE         // RA2L1 can't dump record data during recognition.
#endif

// Definition for group index
#define GROUP_INDEX_TRIGGER      0                       // The group index of trigger.
#define GROUP_INDEX_COMMAND      1                       // The group index of command.


// Definition for audio record.
#define AUDIO_RECORD_I2S         0
#define AUDIO_RECORD_SPI         1
#define AUDIO_RECORD_AMIC        2

#ifndef AUDIO_RECORD
    #define AUDIO_RECORD             AUDIO_RECORD_AMIC
#endif


// Definition for MCU and board type.
#define MCU_BOARD_RA2L1_EK       210    // EK-RA2L1
#define MCU_BOARD_RA2L1_VOICE    220    // VOICE-RA2L1


#if (MCU_BOARD == MCU_BOARD_RA2L1_EK)
	#define MCU_BOARD_STRING                        "RA2L1_EK"
    #define UART_BIT_RATE                           115200                 /* 460800, 921600, 38400 => garbled text */

    /** Data flash */
    #define FLASH_DF_SIZE                           (8192)
    #define FLASH_DF_BLOCK_SIZE                     (BSP_FEATURE_FLASH_LP_DF_BLOCK_SIZE)
    #define FLASH_DF_BLOCK_BASE_ADDRESS             BSP_FEATURE_FLASH_DATA_FLASH_START
    #define FLASH_DF_WRITE_SIZE                     BSP_FEATURE_FLASH_LP_DF_WRITE_SIZE

    /** LED for Voice Command Demo Kit */
    #define LED_R                                   USER_LED3
    #define LED_G                                   USER_LED2
    #define LED_B                                   USER_LED1
    #define OFF                                     (BSP_IO_LEVEL_LOW)
    #define ON                                      (BSP_IO_LEVEL_HIGH)

#elif (MCU_BOARD == MCU_BOARD_RA2L1_VOICE)
	#define MCU_BOARD_STRING                        "RA2L1_VOICE"
    #define UART_BIT_RATE                           460800

    /** Data flash */
    #define FLASH_DF_SIZE                           (8192)
    #define FLASH_DF_BLOCK_SIZE                     (BSP_FEATURE_FLASH_LP_DF_BLOCK_SIZE)
    #define FLASH_DF_BLOCK_BASE_ADDRESS             BSP_FEATURE_FLASH_DATA_FLASH_START
    #define FLASH_DF_WRITE_SIZE                     BSP_FEATURE_FLASH_LP_DF_WRITE_SIZE

    /** LED for Voice Command Demo Kit */
    #define LED_R                                   BSP_IO_PORT_05_PIN_00
    #define LED_G                                   BSP_IO_PORT_02_PIN_13
    #define LED_B                                   BSP_IO_PORT_02_PIN_12
    #define OFF                                     (BSP_IO_LEVEL_HIGH)
    #define ON                                      (BSP_IO_LEVEL_LOW)

#endif

/*******************************************************************************
 Exported global variables
*******************************************************************************/

/*******************************************************************************
 Exported global functions
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void voice_main(void);
void ToggleLED(bsp_io_port_pin_t oLED);
void TurnOnLED(bsp_io_port_pin_t oLED);
void TurnOffLED(bsp_io_port_pin_t oLED);

#ifdef __cplusplus
}
#endif

#endif /* VOICE_MAIN_H */

/*******************************************************************************
 End Of File
*******************************************************************************/
