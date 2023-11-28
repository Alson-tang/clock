#ifndef __DMA_H__
#define __DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stm32f1xx_hal.h"

extern DMA_HandleTypeDef s_dma_handle;
extern bool s_dma_full_transfer;
extern bool s_dma_error;

#define DMA_IT_PREEMPT_PRIORITY	    (0)
#define DMA_IT_SUB_PRIORITY		    (0)

void dma_init(void);
void dma_start(uint32_t *src, uint32_t *dst, uint32_t size);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* dma.h */

