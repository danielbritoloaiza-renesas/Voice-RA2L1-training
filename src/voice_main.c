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

/*******************************************************************************
 Include
*******************************************************************************/

#include "voice_main.h"
#include "hal_data.h"

#include "base_types.h"
#include "DSpotterSDKApi_Const.h"
#include "DSpotterSDKApi.h"
#include "RingBuffer.h"
#include "AGCApi.h"

#include "Convert2TransferBuffer.h"
#include "DbgTrace.h"
#include "UartMgr.h"
#include "FlashMgr.h"
#include "CybModelInfor.h"
#include "Model/CybModel.h"

#ifdef SUPPORT_SPEEX_PLAY
#include "Speex/PlaySpeex.h"
#include "Model/SpeexData.h"
#endif

#include "AudioRecord.h"
#include "lpm_adc.h"

/*******************************************************************************
 Macro definitions
*******************************************************************************/

/** Data flash */
#define FLASH_DF_BLOCK_ADDRESS(nBlockIndex)     (BYTE*)(FLASH_DF_BLOCK_BASE_ADDRESS + nBlockIndex*FLASH_DF_BLOCK_SIZE)
#define FLASH_DF_TOTAL_BLOCKS                   (FLASH_DF_SIZE/FLASH_DF_BLOCK_SIZE)

#define DSPOTTER_LICENSE_MAX_SIZE               256
#define FLASH_DSPOTTER_ADDRESS_OFFSET           0
#define FLASH_DSPOTTER_BLOCK_INDEX              ((FLASH_DSPOTTER_ADDRESS_OFFSET + FLASH_DF_BLOCK_SIZE - 1)/FLASH_DF_BLOCK_SIZE)
#define FLASH_DSPOTTER_BLOCK_COUNT              ((DSPOTTER_LICENSE_MAX_SIZE + FLASH_DF_BLOCK_SIZE - 1)/FLASH_DF_BLOCK_SIZE)

/** DSpotter */
#define DSPOTTER_MEM_SIZE        21256                   // Please modify this number by the return value of CSpotter_GetMemoryUsage_XXX().
#define MAX_COMMAND_TIME         (3000/10)               // Trigger and command must be spoke in 3000 ms (300 frames).
#define RECORD_FRAME_SIZE        320                     // Must be multiple times of 4, 320 is equal to 10 ms data.
#define RECORD_FRAME_SAMPLES     (RECORD_FRAME_SIZE/2)
#define UART_FRAME_TRANSFER_SIZE (RECORD_FRAME_SIZE*5/4) // Every 4-bytes data add one checksum byte
#define VOLUME_SCALE_RECONG      3200                    // The AGC volume scale percentage for recognition. It depends on original microphone data.
#define COMMAND_STAGE_TIME_MIN   6000                    // When no result at command recognition stage, the minimum recording time in ms.
#define COMMAND_STAGE_TIME_MAX   8000                    // When no result at command recognition stage, the maximum recording time in ms.
#define AGC_MEM_SIZE             64                      // AGC memory size.
#define MAP_ID_HELLO_DANIEL      0
#define MAP_ID_LIGHT_RED_ON      0
#define MAP_ID_LIGHT_RED_OFF     1
#define MAP_ID_LIGHT_GREEN_ON    2
#define MAP_ID_LIGHT_GREEN_OFF   3
#define COMMAND_WAIT_BLINK_FRAMES 50

/*******************************************************************************
 Private global functions
*******************************************************************************/

static bool     voice_loop(void);
static void     voice_init(void);
static void     voice_release(void);
static int      TestByteOrder();
static void     SetDSpotter(HANDLE hCyModel, int nGroupIndex, BYTE *pbyaDSpotterMem, int nMemSize, HANDLE *phDSpotter, bool bShowCommandList);
static void     PrintGroupCommandList(HANDLE hCybModel, int nGroupIndex);
static void     OnDataReadCompleteCallback(void);

/*******************************************************************************
 Private global variables
*******************************************************************************/

static HANDLE   g_hDSpotter = NULL;
#if RECOG_FLOW == RECOG_FLOW_NONE
static BYTE     g_byaDSpotterMem[1];                       // The memory for DSpotter engine.
#else
static BYTE     g_byaDSpotterMem[DSPOTTER_MEM_SIZE];       // The memory for DSpotter engine.
#endif
static HANDLE   g_hCybModel = NULL;
static BYTE     g_byaCybModelMem[CYBMODEL_GET_MEM_USAGE()];// The memory for g_hCybModel engine.
static volatile int g_nActiveGroupIndex;
static HANDLE   g_hAGC = NULL;
static BYTE     g_byaAGCMem[AGC_MEM_SIZE];
static int      g_nRecordFrameCount = 0;
static int      g_nLPMRecordFrameCount = 0;
static volatile bool g_bUartCheckAlive = false;
static BYTE     g_byaUartRxBuffer[1];


#ifdef SUPPORT_UART_DUMP_RECORD
#define RBUF_FRAME_COUNT         	4
#define RBUF_SIZE_UART           	(UART_FRAME_TRANSFER_SIZE * RBUF_FRAME_COUNT)   // UART read ring buffer size.
static HANDLE   					g_hRingBufferUart = NULL;                       // The I2S one channel data for recording.
static BYTE     					g_byaRingBufferUart[RING_BUFFER_GET_MEM_USAGE(RBUF_SIZE_UART)];
static volatile bool 				g_bUartSendRecord = true;
static BYTE                         g_byaTxBuffer[UART_FRAME_TRANSFER_SIZE];
#endif


/*******************************************************************************
* Function Name   : voice_main
* Description     : Voice application main process
* Arguments       : none
* Return value    : none
*******************************************************************************/
void voice_main(void)
{
    fsp_err_t   err = FSP_SUCCESS;
    const bsp_unique_id_t   *devi_uniq_id;
    BYTE *lppbyModel[1];
    int  nGroupCount;
    int  nMemSize;

    /** UART */
    err = UartOpen(&g_uart_ds, g_uart_ds.p_cfg->channel, -1);   //baud rate: use configuration.xml setting
    if (FSP_SUCCESS != err)
        DBGTRACE("Failed to open UART %d! error: %d\r\n", g_uart_ds.p_cfg->channel, err);

#ifndef MCU_BOARD
    DBG_UART_TRACE("\r\n[Error] Please define MCU_BOARD first.\r\n");
    __BKPT(0);
#endif

    /** Memory size check, device authentication and ring buffer initialization */
    // Get unique ID
    devi_uniq_id = R_BSP_UniqueIdGet();

    int nByteOrder = TestByteOrder();
    DBG_UART_TRACE("%s\r\n", nByteOrder == 1 ? "Little Endian" : "Big Endian");
    DBG_UART_TRACE("MCU board: %s\r\n", MCU_BOARD_STRING);
    DBG_UART_TRACE("FSP version: %d.%d\r\n", FSP_VERSION_MAJOR, FSP_VERSION_MINOR);
    DBG_UART_TRACE("DSpotter version: %s", DSpotter_VerInfo());
    DBG_UART_TRACE("Sample code build at %s, %s\r\n", __DATE__, __TIME__);
    DBG_UART_TRACE("Unique ID: %X %X %X %X\r\n", devi_uniq_id->unique_id_words[0],
            devi_uniq_id->unique_id_words[1], devi_uniq_id->unique_id_words[2], devi_uniq_id->unique_id_words[3]);

    if (nByteOrder == 0)
    {
        DBG_UART_TRACE("The DSpotter use Renesas RA default compile setting: CM4/HardFPU/little endian, not big endian!\r\n");
        __BKPT(0);
    }

#ifdef SUPPORT_UART_DUMP_RECORD
    memset(g_byaRingBufferUart, 0x00, sizeof(g_byaRingBufferUart));
    if (RingBufferInit(g_byaRingBufferUart, sizeof(g_byaRingBufferUart), RBUF_SIZE_UART, &g_hRingBufferUart) != RING_BUFFER_SUCCESS)
    {
        DBG_UART_TRACE("UART RingBufferInit() fail!\r\n");
        __BKPT(0);
    }
#endif

    g_hCybModel = CybModelInit((const BYTE*)&uCYModelBegin, g_byaCybModelMem, sizeof(g_byaCybModelMem), NULL);

    // Check memory for every group and show their commands.
    DBG_UART_TRACE("The DSpotter declare memory buffer size = %d\r\n", sizeof(g_byaDSpotterMem));
    nGroupCount = CybModelGetGroupCount(g_hCybModel);
    for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
    {
        lppbyModel[0] = (BYTE*)CybModelGetGroup(g_hCybModel, nGroup);
        nMemSize = DSpotter_GetMemoryUsage_Multi((BYTE *)CybModelGetBase(g_hCybModel), lppbyModel, 1, MAX_COMMAND_TIME);
        DBG_UART_TRACE("The model group %d needed working memory size = %d\r\n", nGroup, nMemSize);
        if ((int)sizeof(g_byaDSpotterMem) < nMemSize)
            DBG_UART_TRACE("The DSpotter declare memory buffer size is tool small!\r\n");

        DBG_UART_TRACE("The command list of group index %d: \r\n", nGroup);
        PrintGroupCommandList(g_hCybModel, nGroup);
    }
    DBG_UART_TRACE("\r\n");

    TurnOffLED(LED_R);       //Red     = OFF
    TurnOffLED(LED_G);       //Green   = OFF
    TurnOffLED(LED_B);       //Blue    = OFF

#if defined(SUPPORT_LOW_POWER_MODE) && (AUDIO_RECORD == AUDIO_RECORD_AMIC)
    while(1)
    {
        TurnOffLED(LED_R);       //Red     = OFF

    	lpm_adc_open();
        lpm_adc_enter();

        if (lpm_adc_is_sound_detect())
        {
        	lpm_adc_close();

            voice_init();

            while (voice_loop());

            //Go to here when time out.
            voice_release();

            lpm_adc_reset_sound_detect();
        }
    }
#else
    voice_init();

    while (voice_loop());

    //Go to here when DSPOTTER_ERR_Expired (over trial limit).
    voice_release();
#endif

    CybModelRelease(g_hCybModel);
    g_hCybModel = NULL;

    UartClose();
}
/*******************************************************************************
 End of function voice_main
*******************************************************************************/

/*******************************************************************************
* Function Name: voice_init
* Description  : Initialize
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void voice_init(void)
{
    fsp_err_t   err = FSP_SUCCESS;
    int  nRecogGroupIndex;

    /** LED */
    TurnOffLED(LED_R);       //Red     = OFF
    TurnOffLED(LED_G);       //Green   = OFF
    TurnOffLED(LED_B);       //Blue    = OFF

    //DBG_UART_TRACE("voice_init() Start\r\n");

    nRecogGroupIndex = GROUP_INDEX_TRIGGER;
    SetDSpotter(g_hCybModel, nRecogGroupIndex, g_byaDSpotterMem, sizeof(g_byaDSpotterMem), &g_hDSpotter, false);

#if VOLUME_SCALE_RECONG != 100
    g_hAGC = AGC_Init(g_byaAGCMem, sizeof(g_byaAGCMem), NULL);
    if (g_hAGC == NULL)
    {
        DBG_UART_TRACE("AGC initial fail!\r\n");
        __BKPT(0);
    }
    AGC_SetMaxGain(g_hAGC, VOLUME_SCALE_RECONG/100);
#endif

    err = AudioRecordInit();
    if (FSP_SUCCESS != err)
        __BKPT(0);

    err = AudioRecordStart();
    if (FSP_SUCCESS != err)
        __BKPT(0);

    TurnOffLED(LED_R);    // Light output starts off.
    g_nLPMRecordFrameCount = 0;

    // Process UART read.
	UartAsyncRead(g_byaUartRxBuffer, 1, OnDataReadCompleteCallback);

    //DBG_UART_TRACE("voice_init() End\r\n");
}
/*******************************************************************************
 End of function voice_init
*******************************************************************************/

/*******************************************************************************
* Function Name   : voice_loop
* Description     : Voice application process
* Arguments       : none
* Return value    : none
*******************************************************************************/
static bool voice_loop(void)
{
	short   saRecordSample[RECORD_FRAME_SAMPLES];
    int     nRet = DSPOTTER_ERR_NeedMoreSample;
    static  int s_nCommandRecordSample = 0;
    static  int s_nCommandRecognizeLimit = COMMAND_STAGE_TIME_MIN;
    static  int s_nCommandWaitBlinkFrameCount = 0;

    if (g_bUartCheckAlive)
    {
		const char *lpszAck = "live\r\n";
    	g_bUartCheckAlive = false;
        UartWaitWriteReady();
		UartAsyncWrite((const BYTE *)lpszAck, strlen(lpszAck));
    }

    if (AudioRecordGetDataSize() < RECORD_FRAME_SIZE)
    {
        //__WFI();
        return true;
    }

    g_nLPMRecordFrameCount++;
    if (g_nRecordFrameCount++ % 100 == 0)
    {
	#ifndef SUPPORT_UART_DUMP_RECORD
    	DBG_UART_TRACE(".");
	#endif
    }

    if (g_nActiveGroupIndex == GROUP_INDEX_TRIGGER)
    {
        if (++s_nCommandWaitBlinkFrameCount >= COMMAND_WAIT_BLINK_FRAMES)
        {
            s_nCommandWaitBlinkFrameCount = 0;
            ToggleLED(LED_R);
        }
    }

    // Get recognize data from recognize ring buffer.
    if (AudioRecordGetData((void*)saRecordSample, RECORD_FRAME_SIZE) != 0)
        DBG_UART_TRACE("\r\nAudioRecordGetData() problem \r\n");

    AGC_Do(g_hAGC, saRecordSample, RECORD_FRAME_SAMPLES, saRecordSample);

#ifdef SUPPORT_UART_DUMP_RECORD
    if (g_bUartSendRecord)
    {
        int nTransferSize;

        nTransferSize = Convert2TransferBuffer((BYTE *)saRecordSample, RECORD_FRAME_SIZE, g_byaTxBuffer, sizeof(g_byaTxBuffer), eFourByteDataOneChecksum);
        if (RingBufferPutData(g_hRingBufferUart, g_byaTxBuffer, nTransferSize) != RING_BUFFER_SUCCESS)
        {
            DBG_UART_TRACE("RingBufferPutData() problem!\r\n");
            __BKPT(0);
        }

        if (!UartIsWriting())
        {
            if (RingBufferGetData(g_hRingBufferUart, g_byaTxBuffer, nTransferSize) != RING_BUFFER_SUCCESS)
                return -FSP_ERR_INVALID_SIZE;

            UartAsyncWrite(g_byaTxBuffer, (uint32_t)nTransferSize);
        }
    }
#endif

#if RECOG_FLOW == RECOG_FLOW_NONE
	#if defined(SUPPORT_LOW_POWER_MODE) && (AUDIO_RECORD == AUDIO_RECORD_AMIC)
		if (g_nLPMRecordFrameCount > 300)
		{
			TurnOffLED(LED_R);       //Red     = OFF
			TurnOffLED(LED_G);       //Green   = OFF
			return false;
		}
		else
		{
			return true;
		}
	#else
		return true;
	#endif
#endif

    // DSpotter AddSample
    nRet = DSpotter_AddSample(g_hDSpotter, saRecordSample, RECORD_FRAME_SAMPLES);
    if (nRet == DSPOTTER_SUCCESS)
    {
        int  nCmdIndex, nCmdScore, nCmdSGDiff, nCmdEnergy, nMapID = -1;
        char szCommand[64];

        DSpotter_GetResultScore(g_hDSpotter, &nCmdScore, &nCmdSGDiff, NULL);
        nCmdIndex = DSpotter_GetResult(g_hDSpotter);
        nCmdEnergy = DSpotter_GetCmdEnergy(g_hDSpotter);
        
        CybModelGetCommandInfo(g_hCybModel, g_nActiveGroupIndex, nCmdIndex, szCommand, sizeof(szCommand), &nMapID, NULL);
        #ifdef NOT_SHOW_MULTI_PRONUNCIATION
        if (strchr(szCommand, '^') != NULL)
            strchr(szCommand, '^')[0] = '\0';
        #endif
        DBG_UART_TRACE("\r\nGet command(%d) : %s, Score=%d, SG_Diff=%d, Energy=%d, MapID=%d\r\n",
                nCmdIndex, szCommand, nCmdScore, nCmdSGDiff, nCmdEnergy, nMapID);
        DSpotter_Continue(g_hDSpotter);

#ifdef SUPPORT_SPEEX_PLAY
        const BYTE *pbySpeexDataBegin = (const BYTE *)&g_uSpeexDataBegin;
        int nSampleRate;

        if (PlaySpeexIsDataValid(pbySpeexDataBegin, nMapID, &nSampleRate))
        {
            AudioRecordPause();

            DSpotter_Release(g_hDSpotter);
            g_hDSpotter = NULL;

            PlaySpeexOpenDevice();
            PlaySpeexSetWorkingMemory((char *)g_byaDSpotterMem, sizeof(g_byaDSpotterMem));
            PlaySpeexStart(nSampleRate);
            DBG_UART_TRACE("\r\nStart play.\r\n");
            if (!PlaySpeexMapID(pbySpeexDataBegin, nMapID, 100))
            {
            	DBG_UART_TRACE("Fail to play Speex by MapID(%d).\r\n", nMapID);
            }
	        DBG_UART_TRACE("\r\nStop play.\r\n");
            PlaySpeexStop();
            PlaySpeexCloseDevice();

            SetDSpotter(g_hCybModel, g_nActiveGroupIndex, g_byaDSpotterMem, sizeof(g_byaDSpotterMem), &g_hDSpotter, false);
            // Skip record data during play.
            AudioRecordClearData();
            AudioRecordResume();
        }
#endif
        DBG_UART_TRACE("\r\n");

        DBG_UART_TRACE("Recognized Map ID: %d\r\n", nMapID);
        if (g_nActiveGroupIndex == GROUP_INDEX_TRIGGER && nMapID == MAP_ID_HELLO_DANIEL)
        {
            DBG_UART_TRACE("Wake-up word recognized: Hello Daniel\r\n");
            TurnOffLED(LED_R);
            s_nCommandWaitBlinkFrameCount = 0;
        }
        else if (g_nActiveGroupIndex == GROUP_INDEX_COMMAND)
        {
            s_nCommandWaitBlinkFrameCount = 0;
            TurnOffLED(LED_R);
            switch (nMapID)
            {
            case MAP_ID_LIGHT_RED_ON:
                DBG_UART_TRACE("Light red on command recognized; blue LED on\r\n");
                TurnOnLED(LED_B);
                break;
            case MAP_ID_LIGHT_RED_OFF:
                DBG_UART_TRACE("Light red off command recognized; blue LED off\r\n");
                TurnOffLED(LED_B);
                break;
            case MAP_ID_LIGHT_GREEN_ON:
                DBG_UART_TRACE("Light green on command recognized\r\n");
                TurnOnLED(LED_G);
                break;
            case MAP_ID_LIGHT_GREEN_OFF:
                DBG_UART_TRACE("Light green off command recognized\r\n");
                TurnOffLED(LED_G);
                break;
            default:
                break;
            }
        }

        s_nCommandRecordSample = 0;
        s_nCommandRecognizeLimit = COMMAND_STAGE_TIME_MIN;

        if (g_nActiveGroupIndex == GROUP_INDEX_TRIGGER)
        {
        	// After trigger word recognized, switch to command recognition mode
            SetDSpotter(g_hCybModel, GROUP_INDEX_COMMAND, g_byaDSpotterMem, sizeof(g_byaDSpotterMem), &g_hDSpotter, true);
        }
        // Reset g_nLPMRecordFrameCount.
        g_nLPMRecordFrameCount = 0;
    }
    else if (nRet == DSPOTTER_ERR_Expired)
    {
        DBG_UART_TRACE("\r\nThe trial version DSpotter reach the max trial usage count, please reset system.\r\n");
        TurnOffLED(LED_R);       //Red     = OFF
        TurnOffLED(LED_G);       //Green   = OFF
        return false;
    }
    else if (nRet == DSPOTTER_ERR_NeedMoreSample && g_nActiveGroupIndex == GROUP_INDEX_COMMAND)
    {
        // Check timeout for command recognition mode
        s_nCommandRecordSample += (int)(RECORD_FRAME_SIZE/sizeof(short));
        if (s_nCommandRecordSample > 16000 / 1000 * s_nCommandRecognizeLimit)
        {
            if (DSpotter_IsKeywordAlive(g_hDSpotter))
            {
                if (s_nCommandRecognizeLimit < COMMAND_STAGE_TIME_MAX)
                {
                    s_nCommandRecognizeLimit += 500;
                    return true;
                }
            }
            s_nCommandRecognizeLimit = COMMAND_STAGE_TIME_MIN;

            DBG_UART_TRACE("\r\nTimeout for command stage, switch to trigger stage.\r\n");
            TurnOffLED(LED_R);
            s_nCommandWaitBlinkFrameCount = 0;
            SetDSpotter(g_hCybModel, GROUP_INDEX_TRIGGER, g_byaDSpotterMem, sizeof(g_byaDSpotterMem), &g_hDSpotter, true);
        }
    }

#if defined(SUPPORT_LOW_POWER_MODE) && (AUDIO_RECORD == AUDIO_RECORD_AMIC)

    // Don't sleep at command stage.
    if (g_nActiveGroupIndex == GROUP_INDEX_COMMAND)
        return true;

    if (g_nLPMRecordFrameCount > 100 && !DSpotter_IsKeywordAlive(g_hDSpotter))
    {
        TurnOffLED(LED_R);       //Red     = OFF
        TurnOffLED(LED_G);       //Green   = OFF
    	return false;
    }
#endif

    return true;
}
/*******************************************************************************
 End of function voice_loop
*******************************************************************************/

/*******************************************************************************
* Function Name: voice_release
* Description  : Release resource
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void voice_release(void)
{
    AudioRecordRelease();

    DSpotter_Release(g_hDSpotter);
    g_hDSpotter = NULL;

    AGC_Release(g_hAGC);
    g_hAGC = NULL;
}
/*******************************************************************************
 End of function voice_release
*******************************************************************************/

static int TestByteOrder()
{
    short nTestWord = 0x0001;      //Little endian memory is 0x01 0x00, big endian memory is 0x00 0x01.
    char *b = (char *)&nTestWord;  //Little endian b[0] is 0x01, big endian memory b[0] is 0x00.
    return (int)b[0];              //Return 1/0 for little/big endian.
}

static void SetDSpotter(HANDLE hCybModel, int nGroupIndex, BYTE *pbyaDSpotterMem, int nMemSize, HANDLE *phDSpotter, bool bShowCommandList)
{
    int nRet = DSPOTTER_ERR_IllegalParam;
    BYTE *lppbyModel[2];
    int nModelCount = 1;
    int nGroupCount = CybModelGetGroupCount(hCybModel);
    int nCommandCount;

#if RECOG_FLOW == RECOG_FLOW_NONE
    return;
#endif

    if (nGroupIndex >= nGroupCount)
    {
        DBG_UART_TRACE("Invalid nGroupIndex parameter!\r\n");
    	return;
    }

    if (phDSpotter != NULL)
    {
        DSpotter_Release(*phDSpotter);
        *phDSpotter = NULL;
    }

    nCommandCount = CybModelGetCommandCount(hCybModel, nGroupIndex);
    if (nCommandCount <= 20)
    	DSpotter_SetFastMode(0);
    else
    	DSpotter_SetFastMode(1);

    /*
     * For official version of DSpotter, DSpotter_Init_Multi() will use the data flash API and
     * its instance to check license. In configuration.xml, please:
     *   1. Add r_flash_hp component.
     *   2. Add Flash Driver stack, named "g_flash0" and set callback as "flash0_bgo_callback".
     * For RA6M1, The data flash address start at 0x40100000U, total 128 blocks, each block has
     * 64 bytes, total 8K bytes. DSpotter need 256 bytes(4 blocks) to save license information.
     * So, the valid flash address is FLASH_DF_BLOCK_ADDRESS(0) ~ FLASH_DF_BLOCK_ADDRESS(123).
    */
    if (FLASH_DSPOTTER_BLOCK_INDEX + FLASH_DSPOTTER_BLOCK_COUNT > FLASH_DF_TOTAL_BLOCKS)
    {
        DBG_UART_TRACE("FLASH_DSPOTTER_BLOCK_INDEX error!\r\n");
        __BKPT(0);
    }

    lppbyModel[0] = (BYTE*)CybModelGetGroup(hCybModel, nGroupIndex);
    *phDSpotter = DSpotter_Init_Multi((BYTE *)CybModelGetBase(hCybModel), lppbyModel, nModelCount, MAX_COMMAND_TIME,
                                      pbyaDSpotterMem, nMemSize, FLASH_DF_BLOCK_ADDRESS(FLASH_DSPOTTER_BLOCK_INDEX), FLASH_DF_BLOCK_SIZE, &nRet);

    if (*phDSpotter == NULL)
    {
        DBG_UART_TRACE("DSpotter_Init_XXX() fail, error = %d!\r\n", nRet);
        __BKPT(0);
    }
    else
    {
	#if defined(SUPPORT_LOW_POWER_MODE) && (AUDIO_RECORD == AUDIO_RECORD_AMIC)
    	if (nGroupIndex == GROUP_INDEX_TRIGGER)
    	{
    		DSpotter_SetOptionBeginStateTime(*phDSpotter, 2);
        	for (int i = 0; i < nCommandCount; i++)
        		DSpotter_SetWordOptionBeginState(*phDSpotter, i, 4);
    	}
	#endif

        if (nCommandCount > 20)
        {
        	int nCmdConfiReward;
        	for (int i = 0; i < nCommandCount; i++)
        	{
        		nCmdConfiReward = DSpotter_GetCmdConfiReward(*phDSpotter, i, &nRet);
        		if (nCmdConfiReward > -100 && nRet == DSPOTTER_SUCCESS)
        			DSpotter_SetCmdConfiReward(*phDSpotter, i, nCmdConfiReward + 5);
        	}
        }

        DBG_UART_TRACE("%s group active.\r\n", nGroupIndex == 0 ? "Wake-up" : "Command");

        if (bShowCommandList)
        	PrintGroupCommandList(hCybModel, nGroupIndex);
    }
    g_nActiveGroupIndex = nGroupIndex;
}

void PrintGroupCommandList(HANDLE hCybModel, int nGroupIndex)
{
	char szCommand[64];
	int  nMapID;

	for (int i = 0; i < CybModelGetCommandCount(hCybModel, nGroupIndex); i++)
	{
		CybModelGetCommandInfo(hCybModel, nGroupIndex, i, szCommand, sizeof(szCommand), &nMapID, NULL);
		if (strlen(szCommand) > 0)
		{
		#ifdef NOT_SHOW_MULTI_PRONUNCIATION
			if (strchr(szCommand, '^') != NULL)
				continue;
		#endif
			DBG_UART_TRACE("    %s, Map ID = %d\r\n", szCommand, nMapID);
		}
	}
	DBG_UART_TRACE("\r\n");
}

void ToggleLED(bsp_io_port_pin_t oLED)
{
    bsp_io_level_t level;

    R_IOPORT_PinRead(&g_ioport_ctrl, oLED, &level);
    if (level == ON)
        R_IOPORT_PinWrite(&g_ioport_ctrl, oLED, OFF);
    else
        R_IOPORT_PinWrite(&g_ioport_ctrl, oLED, ON);
}

void TurnOnLED(bsp_io_port_pin_t oLED)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, oLED, ON);
}

void TurnOffLED(bsp_io_port_pin_t oLED)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, oLED, OFF);
}

void OnDataReadCompleteCallback(void)
{
    static char s_szCmd[4] = { 0 };
    static int8_t s_nIndex = 0;
    char ch = (char)g_byaUartRxBuffer[0];

    if (s_nIndex < 4)
    {
    	s_szCmd[s_nIndex++] = ch;
    }
    else
    {
    	memmove(s_szCmd, s_szCmd + 1, 3);
    	s_szCmd[3] = ch;
    	if (strncmp(s_szCmd, "CYB", 3) == 0)
    	{
		#ifdef SUPPORT_UART_DUMP_RECORD
			if (ch == 0x31)
				g_bUartSendRecord = true;
			else if (ch == 0x32)
				g_bUartSendRecord = false;
		#endif
			if (ch == 0x33)
				g_bUartCheckAlive = true;
    	}
    }

    //Continue to read next byte.
    UartAsyncRead(g_byaUartRxBuffer, 1, OnDataReadCompleteCallback);
}
/*******************************************************************************
 End Of File
*******************************************************************************/
