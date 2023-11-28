#ifndef __W25QXX_H__
#define __W25QXX_H__

#include "stm32f1xx_hal.h"

/************************* SPI Flash 相关定义 *************************/
#define W25Q64

#if defined W25Q16
#define FLASH_ID                        0xEF3015
#elif defined W25Q64                
#define FLASH_ID                        0XEF4017
#elif defined W25Q128               
#define FLASH_ID                        0XEF4018
#endif 

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

#define SPI_FLASH_BLOCK_SIZE            (64 * 1024)

#define W25X_WriteEnable		        0x06 
#define W25X_WriteDisable		        0x04 
#define W25X_ReadStatusReg		        0x05 
#define W25X_WriteStatusReg		        0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		        0x0B 
#define W25X_FastReadDual		        0x3B 
#define W25X_PageProgram		        0x02 
#define W25X_BlockErase			        0xD8 
#define W25X_SectorErase		        0x20 
#define W25X_ChipErase			        0xC7 
#define W25X_PowerDown			        0xB9 
#define W25X_ReleasePowerDown	        0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	    0x90 
#define W25X_JedecDeviceID		        0x9F

#define WIP_Flag                        0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                      0x55

typedef struct w25q64_cfg
{
    void (*write)(uint8_t * buf, uint16_t len);
    void (*read)(uint8_t * buf, uint16_t len);
    uint8_t (*read_one_byte)(uint8_t send_byte);
    void (*cs_select)(void);
    void (*cs_unselect)(void);
} w25q64_cfg_t;

void w25q64_init(w25q64_cfg_t *w25q64_cfg);

/************************* SPI Flash 相关定义结束 *************************/
/*
 *  函数名：uint32_t FLASH_ReadDeviceID(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到外部FLASH的设备ID
 *  函数作用：读外部FLASH的设备ID
*/
extern uint32_t FLASH_ReadDeviceID(void);

/*
 *  函数名：uint32_t Flash_ReadFlashID(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到外部FLASH的芯片ID
 *  函数作用：读外部FLASH的芯片ID
*/
extern uint32_t Flash_ReadFlashID(void);

/*
 *  函数名：void Flash_PowerDown(void)  
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：断开FLASH
*/
extern void Flash_PowerDown(void);

/*
 *  函数名：void Flash_WakeUp(void) 
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：唤醒FLASH
*/
extern void Flash_WakeUp(void);

/*
 *  函数名：void FLASH_SectorErase(uint32_t SectorAddr)
 *  输入参数：SectorAddr -> 要擦擦的地址
 *  输出参数：无
 *  返回值：无
 *  函数作用：扇区擦除
*/
extern void FLASH_SectorErase(uint32_t SectorAddr);

/*
 *  函数名：void FLASH_BlockErase(uint32_t BlockAddr)
 *  输入参数：BlockAddr -> 要擦的块地址
 *  输出参数：无
 *  返回值：无
 *  函数作用：块擦除
*/
extern void FLASH_BlockErase(uint32_t BlockAddr);

/*
 *  函数名：void FLASH_ChipErase(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：芯片全擦除
*/
extern void FLASH_ChipErase(void);

/*
 *  函数名：void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
 *  输入参数：pBuffer -> 要写的数据指针;    WriteAddr -> 要写的FLASH初始地址;  NumByteToWrite -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：页写
*/
extern void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

/*
 *  函数名：void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite))
 *  输入参数：pBuffer -> 要写的数据指针;    WriteAddr -> 要写的FLASH初始地址;  NumByteToWrite -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：写N个字节
*/
extern void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

/*
 *  函数名：void FLASH_BufferRead(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite))
 *  输入参数：pBuffer -> 要读的数据指针;    WriteAddr -> 要读的FLASH初始地址;  NumByteToWrite -> 要读的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：读N个字节出来
*/
extern void FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
extern void FLASH_StartReadSequence(uint32_t ReadAddr);
#endif
