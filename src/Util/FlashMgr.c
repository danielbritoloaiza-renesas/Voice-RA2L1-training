#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "voice_main.h"
#include "FlashMgr.h"
#include "DbgTrace.h"
#include "UartMgr.h"
#include "PortFunction.h"
#if BSP_FEATURE_FLASH_LP_VERSION != 0
	#include "r_flash_lp.h"
#else
	#include "r_flash_hp.h"
#endif

#define FLASH_SIZE_TO_BLOCK_COUNT(uSize)        ((uSize + FLASH_DF_BLOCK_SIZE - 1)/FLASH_DF_BLOCK_SIZE)
#define FLASH_ADDR_TO_BLOCK_INDEX(Addr)         ((((uint32_t)Addr) - FLASH_DF_BLOCK_BASE_ADDRESS)/FLASH_DF_BLOCK_SIZE)
#define FLASH_BLOCK_INDEX_TO_ADDR(nIndex)       (void *)(nIndex*FLASH_DF_BLOCK_SIZE + FLASH_DF_BLOCK_BASE_ADDRESS)

#ifndef CM4
    #define CM4     4
#endif
#ifndef CM23
    #define CM23   23
#endif
#ifndef CM33
    #define CM33   33
#endif
#ifndef CM85
    #define CM85   85
#endif

extern const flash_instance_t g_flash0;

static const flash_instance_t *g_pFlashInst = NULL;
static volatile bool g_b_flash_event_not_blank = false;
static volatile bool g_b_flash_event_blank = false;
static volatile bool g_b_flash_event_erase_complete = false;
static volatile bool g_b_flash_event_write_complete = false;


void flash0_bgo_callback(flash_callback_args_t *p_args);

int FlashGetDataSize(void)
{
	return FLASH_DF_SIZE;
}

int FlashGetBlockSize(void)
{
	return FLASH_DF_BLOCK_SIZE;
}

void* FlashGetBaseAddress(void)
{
	return (void *)FLASH_DF_BLOCK_BASE_ADDRESS;
}

void* FlashGetBlockAddress(int nBlockIndex)
{
	if (nBlockIndex < 0 || nBlockIndex >= (int)(FLASH_DF_SIZE / FLASH_DF_BLOCK_SIZE))
		return NULL;

	return (void *)(FLASH_DF_BLOCK_BASE_ADDRESS + (uint32_t)nBlockIndex*FLASH_DF_BLOCK_SIZE);
}

int FlashOpen(void)
{
    int nRet = FSP_SUCCESS;

    if (g_pFlashInst != NULL)
        return FSP_ERR_ALREADY_OPEN;

    g_pFlashInst = &g_flash0;

#if BSP_FEATURE_FLASH_LP_VERSION != 0
    nRet = R_FLASH_LP_Open(g_pFlashInst->p_ctrl, g_pFlashInst->p_cfg);
#else
    nRet = R_FLASH_HP_Open(g_pFlashInst->p_ctrl, g_pFlashInst->p_cfg);
#endif

    if (nRet != FSP_SUCCESS)
    {
        g_pFlashInst = NULL;
        DbgUartTrace("R_FLASH_HP_Open() err = %d!\n", nRet);
    }

    return nRet;
}


int FlashClose(void)
{
    int nRet;

    if (g_pFlashInst == NULL)
        return FSP_SUCCESS;

#if BSP_FEATURE_FLASH_LP_VERSION != 0
    nRet = R_FLASH_LP_Close(g_pFlashInst->p_ctrl);
#else
    nRet = R_FLASH_HP_Close(g_pFlashInst->p_ctrl);
#endif

    if (nRet != FSP_SUCCESS)
        DbgUartTrace("R_FLASH_HP_Close() err = %d!\n", nRet);

    g_pFlashInst = NULL;

    return nRet;
}


bool FlashIsOpened(void)
{
    return (g_pFlashInst != NULL);
}


int FlashErase(void *lpFlashBlockAddress, int nNumBlocks)
{
    int nRet = FSP_SUCCESS;
    int nCount;

    while (true)
    {
        if (g_pFlashInst == NULL)
        {
            nRet = FSP_ERR_NOT_OPEN;
            break;
        }

        // Check the low boundary of data flash.
        if ((uint32_t)lpFlashBlockAddress < FLASH_DF_BLOCK_BASE_ADDRESS)
        {
            nRet = FSP_ERR_INVALID_ADDRESS;
            break;
        }

        // Check the high boundary of data flash.
        if ((uint32_t)lpFlashBlockAddress + (uint32_t)nNumBlocks*FLASH_DF_BLOCK_SIZE > FLASH_DF_BLOCK_BASE_ADDRESS + FLASH_DF_SIZE)
        {
            nRet = FSP_ERR_INVALID_ARGUMENT;
            break;
        }

        // Check the address is the start of block.
        if (((uint32_t)lpFlashBlockAddress - FLASH_DF_BLOCK_BASE_ADDRESS) % FLASH_DF_BLOCK_SIZE != 0)
        {
            nRet = FSP_ERR_INVALID_ARGUMENT;
            break;
        }

        g_b_flash_event_erase_complete = false;
    #if BSP_FEATURE_FLASH_LP_VERSION != 0
        nRet = R_FLASH_LP_Erase(g_pFlashInst->p_ctrl, (uint32_t)lpFlashBlockAddress, (uint32_t)nNumBlocks);
    #else
        nRet = R_FLASH_HP_Erase(g_pFlashInst->p_ctrl, (uint32_t)lpFlashBlockAddress, (uint32_t)nNumBlocks);
    #endif

        if (FSP_SUCCESS != nRet)
            break;

        // Wait for the erase complete event flag, if BGO is SET.
        if (true == g_pFlashInst->p_cfg->data_flash_bgo)
        {
            //UartTrace("BGO has enabled\r\n");
            nCount = 0;
            while (!g_b_flash_event_erase_complete)
            {
                PortDelay(1);
                if (nCount++ >= 3000)
                    break;
            }
            if (nCount >= 3000)
                nRet = FSP_ERR_TIMEOUT;
        }

        break;
    }

    if (FSP_SUCCESS != nRet)
        DbgUartTrace("FlashErase() failed! %d\r\n", nRet);

    return nRet;
}

int FlashBlankCheck(void *lpFlashAddress, int nNumBytes)
{
    int nRet = FSP_SUCCESS;
    int nCount;
    flash_result_t blank_check_result = 0;

    while (true)
    {
        if (g_pFlashInst == NULL)
        {
            nRet = FSP_ERR_NOT_OPEN;
            break;
        }

        if ((uint32_t)lpFlashAddress < FLASH_DF_BLOCK_BASE_ADDRESS)
        {
            nRet = FSP_ERR_INVALID_ADDRESS;
            break;
        }

        if ((uint32_t)lpFlashAddress + (uint32_t)nNumBytes > FLASH_DF_BLOCK_BASE_ADDRESS + FLASH_DF_SIZE)
        {
            nRet = FSP_ERR_INVALID_ARGUMENT;
            break;
        }

        g_b_flash_event_blank = false;
        g_b_flash_event_not_blank = false;
    #if BSP_FEATURE_FLASH_LP_VERSION != 0
        nRet = R_FLASH_LP_BlankCheck(g_pFlashInst->p_ctrl, (uint32_t)lpFlashAddress, (uint32_t)nNumBytes, &blank_check_result);
    #else
        nRet = R_FLASH_HP_BlankCheck(g_pFlashInst->p_ctrl, (uint32_t)lpFlashAddress, (uint32_t)nNumBytes, &blank_check_result);
    #endif

        if (FSP_SUCCESS != nRet)
            break;

        // Validate the blank check result.
        if (FLASH_RESULT_BLANK == blank_check_result)
        {
            //DbgUartTrace("BlankCheck is successful\r\n");
        }
        else if (FLASH_RESULT_NOT_BLANK == blank_check_result)
        {
            //DbgUartTrace("BlankCheck is not blank, not to write the data!\r\n");
            nRet = (int)FLASH_RESULT_NOT_BLANK;
        }
        else if (FLASH_RESULT_BGO_ACTIVE == blank_check_result)
        {
            // BlankCheck will update in Callback
            // Event flag will be updated in the blank check function when BGO is enabled.

            // Wait for callback function to set flag.
            nCount = 0;
            while (!(g_b_flash_event_not_blank || g_b_flash_event_blank))
            {
                PortDelay(1);
                if (nCount++ >= 3000)
                    break;
            }
            if (nCount >= 3000)
            {
                DbgUartTrace("R_FLASH_HP_BlankCheck() timeout failed!\r\n");
                nRet = (int)FLASH_STATUS_BUSY;
            }

            if (g_b_flash_event_not_blank)
            {
                //DbgUartTrace("Flash is not blank, need erase before writing data!\n\r");
                nRet = (int)FLASH_RESULT_NOT_BLANK;
            }
            else if (g_b_flash_event_blank)
            {
                nRet = (int)FSP_SUCCESS;
            }
        }
        else
        {
            // No Operation, should not go to here.
        }

        break;
    }

    return nRet;
}


int FlashWrite(void *lpFlashAddress, const uint8_t* lpData, int nDataSize)
{
    int nRet = FSP_SUCCESS;
    int nCount;
    int nCmp;

    while (true)
    {
        if (g_pFlashInst == NULL)
        {
            nRet = FSP_ERR_NOT_OPEN;
            break;
        }

        if ((uint32_t)lpFlashAddress < FLASH_DF_BLOCK_BASE_ADDRESS)
        {
            nRet = FSP_ERR_INVALID_ADDRESS;
            break;
        }

        if ((uint32_t)lpFlashAddress + (uint32_t)nDataSize > FLASH_DF_BLOCK_BASE_ADDRESS + FLASH_DF_SIZE)
        {
            nRet = FSP_ERR_INVALID_ARGUMENT;
            break;
        }
	
	#if _RA_CORE==CM85
	#ifdef D_CACHE_ENABLE
	    SCB_CleanDCache_by_Addr((uint8_t*)lpData, (int32_t)ALIGN_BASE2_CEIL((size_t)nDataSize, __SCB_DCACHE_LINE_SIZE));
	#endif
	#endif

        g_b_flash_event_write_complete = false;
    #if BSP_FEATURE_FLASH_LP_VERSION != 0
        nRet = R_FLASH_LP_Write(g_pFlashInst->p_ctrl, (uint32_t)lpData, (uint32_t)lpFlashAddress, (uint32_t)nDataSize);
    #else
        nRet = R_FLASH_HP_Write(g_pFlashInst->p_ctrl, (uint32_t)lpData, (uint32_t)lpFlashAddress, (uint32_t)nDataSize);
    #endif

        if (FSP_SUCCESS != nRet)
            break;

        // Wait for the write complete event flag, if BGO is SET.
        if (true == g_pFlashInst->p_cfg->data_flash_bgo)
        {
            nCount = 0;
            while (!g_b_flash_event_write_complete)
            {
                PortDelay(1);
                if (nCount++ >= 3000)
                    break;
            }
            if (nCount >= 3000)
            {
                nRet = FLASH_STATUS_BUSY;
                break;
            }
        }
        //DbgUartTrace("Writing flash data is successful.\r\n");

#if _RA_CORE==CM85
	    uint8_t p_read_buffer[__SCB_DCACHE_LINE_SIZE];
	    int nIndex;
	    size_t uCount;

#ifdef D_CACHE_ENABLE
    	SCB_CleanInvalidateDCache_by_Addr((uint8_t *) (uint32_t)lpFlashAddress, (int32_t)ALIGN_BASE2_CEIL((size_t)nDataSize, __SCB_DCACHE_LINE_SIZE));
#endif
	    nIndex = 0;
	    while (nDataSize > 0)
	    {
	        /*Read Data Flash data */
	    	if (nDataSize >= (int)sizeof(p_read_buffer))
	    		uCount = sizeof(p_read_buffer);
	    	else
	    		uCount = (size_t)nDataSize;
	        memcpy(p_read_buffer, (uint8_t *)lpFlashAddress + nIndex, uCount);
			nCmp = memcmp(p_read_buffer, lpData + nIndex, uCount);
	        if (0 != nCmp)
	        {
	            DbgUartTrace("Read and Write buffer is verified fail!\r\n");
	            return FSP_ERR_WRITE_FAILED;
	        }
	        nIndex += (int)uCount;
	        nDataSize -= (int)uCount;
	    }
#else
        // Comparing the write_buffer and read_buffer
	    nCmp = memcmp((uint8_t *)(uint32_t)lpFlashAddress, lpData, (size_t)nDataSize);
	    if (0 != nCmp)
        {
            DbgUartTrace("FlashWrite: Read and Write buffer is verified fail!\r\n");
            nRet = FSP_ERR_WRITE_FAILED;
        }
#endif

        break;
    }

    if (FSP_SUCCESS != nRet)
        DbgUartTrace("FlashWrite() failed! %d\r\n", nRet);

    return nRet;
}

int FlashSmartWrite(void *lpFlashAddress, const uint8_t *lpbyData, int nDataSize)
{
    int nRet = FSP_SUCCESS;
    uint8_t byaBlock[FLASH_DF_BLOCK_SIZE];
    uint8_t *lpbyFlashAddress;
    uint32_t nBlockIndex;
    int nSize;
    bool bFlashIsOpened = FlashIsOpened();
#if _RA_CORE==CM85
    const uint8_t *lpData = lpbyData;
#endif

    if (!bFlashIsOpened)
        FlashOpen();

    // Entering to data flash operations.
    while (true)
    {
        if (g_pFlashInst == NULL)
        {
            nRet = FSP_ERR_NOT_OPEN;
            break;
        }

        if ((uint32_t)lpFlashAddress < FLASH_DF_BLOCK_BASE_ADDRESS)
        {
            nRet = FSP_ERR_INVALID_ADDRESS;
            break;
        }

        if ((uint32_t)lpFlashAddress % FLASH_DF_WRITE_SIZE != 0)
        {
            nRet = FSP_ERR_INVALID_ADDRESS;
            break;
        }

        if ((uint32_t)lpFlashAddress + (uint32_t)nDataSize > FLASH_DF_BLOCK_BASE_ADDRESS + FLASH_DF_SIZE)
        {
            nRet = FSP_ERR_INVALID_ARGUMENT;
            break;
        }

        lpbyFlashAddress = (uint8_t *)lpFlashAddress;
        // Handle the case of lpbyFlashAddress not align on block boundary.
        if ( ((uint32_t)lpFlashAddress - FLASH_DF_BLOCK_BASE_ADDRESS) % FLASH_DF_BLOCK_SIZE != 0)
        {
            nBlockIndex = FLASH_ADDR_TO_BLOCK_INDEX((uint32_t)lpbyFlashAddress);
            nSize = (int)((uint32_t)FLASH_BLOCK_INDEX_TO_ADDR(nBlockIndex + 1) - (uint32_t)lpbyFlashAddress);
            if (nSize > nDataSize)
                nSize = nDataSize;
            memcpy(byaBlock, FLASH_BLOCK_INDEX_TO_ADDR(nBlockIndex), FLASH_DF_BLOCK_SIZE);
            memcpy(byaBlock + FLASH_DF_BLOCK_SIZE - (uint32_t)nSize, lpbyData, (uint32_t)nSize);
            nRet = FlashErase(FLASH_BLOCK_INDEX_TO_ADDR(nBlockIndex), 1);
            nRet = FlashWrite(FLASH_BLOCK_INDEX_TO_ADDR(nBlockIndex), byaBlock, FLASH_DF_BLOCK_SIZE);

            lpbyFlashAddress += nSize;
            lpbyData += nSize;
            nDataSize -= nSize;
        }

        if (nDataSize <= 0 || FSP_SUCCESS != nRet)
            break;

        // lpbyFlashAddress will be align to block boundary when go to here.

	#if _RA_CORE==CM85
	#ifdef D_CACHE_ENABLE
	    SCB_CleanDCache_by_Addr((uint8_t*)lpData, (int32_t)ALIGN_BASE2_CEIL((size_t)nDataSize, __SCB_DCACHE_LINE_SIZE));
	#endif
	#endif
	
        if (nDataSize > (int)FLASH_DF_BLOCK_SIZE)
        {
            nSize = (nDataSize / (int)FLASH_DF_BLOCK_SIZE) * (int)FLASH_DF_BLOCK_SIZE;

            // Blank check for write area.
            nRet = FlashBlankCheck(lpbyFlashAddress, nSize);
            if (FLASH_RESULT_NOT_BLANK == nRet)
            {
                // Erase Block
                nRet = FlashErase(lpbyFlashAddress, nDataSize/(int)FLASH_DF_BLOCK_SIZE);
                if (FSP_SUCCESS != nRet)
                    break;
            }
            else if (FSP_SUCCESS != nRet)
            {
                break;
            }

            // Write code flash data.
            nRet = FlashWrite(lpbyFlashAddress, lpbyData, nSize);
            if (FSP_SUCCESS != nRet)
                break;

            lpbyFlashAddress += nSize;
            lpbyData += nSize;
            nDataSize -= nSize;
        }

        if (nDataSize <= 0)
            break;

        // When go to here, nDataSize must be less than FLASH_DF_BLOCK_SIZE.
        memcpy(byaBlock, lpbyFlashAddress, FLASH_DF_BLOCK_SIZE);
        memcpy(byaBlock, lpbyData, (uint32_t)nDataSize);
        nRet = FlashErase(lpbyFlashAddress, 1);
        nRet = FlashWrite(lpbyFlashAddress, byaBlock, FLASH_DF_BLOCK_SIZE);

        break;
    }

#if _RA_CORE==CM85
#ifdef D_CACHE_ENABLE
   	SCB_CleanInvalidateDCache_by_Addr((uint8_t *) (uint32_t)lpFlashAddress, (int32_t)ALIGN_BASE2_CEIL((size_t)nDataSize, __SCB_DCACHE_LINE_SIZE));
#endif
#endif

    if (!bFlashIsOpened)
        FlashClose();

    return nRet;
}


// Callback function for FLASH HP HAL
void flash0_bgo_callback(flash_callback_args_t *p_args)
{
    if (FLASH_EVENT_NOT_BLANK == p_args->event)
    {
        g_b_flash_event_not_blank = true;
    }
    else if (FLASH_EVENT_BLANK == p_args->event)
    {
        g_b_flash_event_blank = true;
    }
    else if (FLASH_EVENT_ERASE_COMPLETE == p_args->event)
    {
        g_b_flash_event_erase_complete = true;
    }
    else if (FLASH_EVENT_WRITE_COMPLETE == p_args->event)
    {
        g_b_flash_event_write_complete = true;
    }
    else
    {
        // No operation
    }
}



