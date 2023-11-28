#include "dma.h"

DMA_HandleTypeDef s_dma_handle = { 0 };
bool s_dma_full_transfer = false;
bool s_dma_error = false;

void dma_full_transfer_cb(DMA_HandleTypeDef * _hdma)
{
    s_dma_full_transfer = true;
}

void dma_error_cb(DMA_HandleTypeDef * _hdma)
{
    s_dma_error = true;
}


void dma_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();

    s_dma_handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
    s_dma_handle.Init.PeriphInc = DMA_PINC_ENABLE;
    s_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    s_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    s_dma_handle.Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
    s_dma_handle.Init.Mode = DMA_NORMAL;
    s_dma_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    s_dma_handle.Instance = DMA1_Channel1;

    HAL_DMA_Init(&s_dma_handle);

    HAL_DMA_RegisterCallback(&s_dma_handle, HAL_DMA_XFER_CPLT_CB_ID, dma_full_transfer_cb);
    HAL_DMA_RegisterCallback(&s_dma_handle, HAL_DMA_XFER_ERROR_CB_ID, dma_error_cb);

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, DMA_IT_PREEMPT_PRIORITY, DMA_IT_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void dma_start(uint32_t *src, uint32_t *dst, uint32_t size)
{
    s_dma_full_transfer = false;
    s_dma_error = false;

    HAL_DMA_Start_IT(&s_dma_handle, (uint32_t)src, (uint32_t)dst, size);
}
