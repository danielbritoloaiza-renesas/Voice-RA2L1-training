#include <stdlib.h>
#include "hal_data.h"
#include "PortFunction.h"
#include "DbgTrace.h"

#ifdef _FREERTOS_

#include "FreeRTOS.h"
#include "task.h"

void SoftwareDelayMillSec(uint32_t delay_in_ms)
{
    vTaskDelay(pdMS_TO_TICKS(delay_in_ms));
}

void *PortMalloc(size_t size)
{
    void *p;

    p = pvPortMalloc(size);
    if(p==NULL)
    {
        DbgUartTrace("ERROR: PortCalloc(%d) fail!\r\n", size);
    }
    return p;
}

void *PortCalloc(size_t num, size_t size)
{
    void *p;

    /* allocate 'count' objects of size 'size' */
    p = pvPortMalloc(num * size);
    if (p) {
      /* zero the memory */
      memset(p, 0, num * size);
    }
    else
    {
        DbgUartTrace("ERROR: PortCalloc(%d) fail!\r\n", size);
    }
    return p;
}

void *PortRealloc(void *ptr, size_t size)
{
    if (size == 0)
    {
        vPortFree(ptr);
        return NULL;
    }

    void *p;
    p = pvPortMalloc(size);
    if (p) {
        /* zero the memory */
        if (ptr != NULL) {
            memcpy(p, ptr, size);
            vPortFree(ptr);
        }
    }
    else
    {
        DbgUartTrace("ERROR: PortRealloc(%d) fail!\r\n", size);
    }

    return p;
}

void  PortFree(void *ptr)
{
    vPortFree(ptr);
}

void PortSleep(uint32_t delay_in_ms)
{
    if (delay_in_ms == 0) {
        taskYIELD();
    }
    else {
        vTaskDelay(pdMS_TO_TICKS(delay_in_ms));
    }
}

void PortDelay(uint32_t delay_in_ms)
{
    vTaskDelay(pdMS_TO_TICKS(delay_in_ms));
}


#else

void SoftwareDelayMillSec(uint32_t delay_in_ms)
{
    R_BSP_SoftwareDelay(delay_in_ms, BSP_DELAY_UNITS_MILLISECONDS);
}

void *PortMalloc(size_t size)
{
    void *p = malloc(size);

    if (!p)
        DbgUartTrace("ERROR: PortMalloc(%d) fail!\r\n", size);
    return p;
}

void *PortCalloc(size_t num, size_t size)
{
    void *p = calloc(num, size);

    if (!p)
        DbgUartTrace("ERROR: PortCalloc(%d) fail!\r\n", size);
    return p;
}

void *PortRealloc(void *ptr, size_t size)
{
    void *p = realloc(ptr, size);

    if (!p)
        DbgUartTrace("ERROR: PortMalloc(%d) fail!\r\n", size);
    return p;
}

void  PortFree(void *ptr)
{
    free(ptr);
}

void PortSleep(uint32_t delay_in_ms)
{
    FSP_PARAMETER_NOT_USED(delay_in_ms);
	// Not support for non-OS platform.
}

void PortDelay(uint32_t delay_in_ms)
{
    R_BSP_SoftwareDelay(delay_in_ms, BSP_DELAY_UNITS_MILLISECONDS);
}

#endif
