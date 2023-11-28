#include "w25q64.h"

static w25q64_cfg_t * sp_w25q64_cfg = NULL;

void w25q64_init(w25q64_cfg_t *w25q64_cfg)
{
    if (w25q64_cfg == NULL) {
        return;
    }

    sp_w25q64_cfg = w25q64_cfg;

    return;
}

/*
 *  函数名：uint32_t FLASH_ReadDeviceID(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到外部FLASH的设备ID
 *  函数作用：读外部FLASH的设备ID
*/
uint32_t FLASH_ReadDeviceID(void)
{
    uint32_t temp[4];

    sp_w25q64_cfg->cs_select();

    sp_w25q64_cfg->read_one_byte(W25X_DeviceID);
    temp[0] = sp_w25q64_cfg->read_one_byte(Dummy_Byte);
    temp[1] = sp_w25q64_cfg->read_one_byte(Dummy_Byte);
    temp[2] = sp_w25q64_cfg->read_one_byte(Dummy_Byte);
    temp[3] = sp_w25q64_cfg->read_one_byte(Dummy_Byte); //deviceID
	
    sp_w25q64_cfg->cs_unselect();

    return temp[3];
}

/*
 *  函数名：uint32_t Flash_ReadFlashID(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到外部FLASH的芯片ID
 *  函数作用：读外部FLASH的芯片ID
*/
uint32_t Flash_ReadFlashID(void)
{
    uint32_t temp[4];
    
    sp_w25q64_cfg->cs_select();
    
    sp_w25q64_cfg->read_one_byte(W25X_JedecDeviceID);
    temp[0] = sp_w25q64_cfg->read_one_byte(Dummy_Byte);
    temp[1] = sp_w25q64_cfg->read_one_byte(Dummy_Byte);
    temp[2] = sp_w25q64_cfg->read_one_byte(Dummy_Byte);
    
    sp_w25q64_cfg->cs_unselect();
	
    temp[3] = (temp[0] << 16) | (temp[1] << 8) | temp[2];

    return temp[3];
}

/*
 *  函数名：static void Flash_WritenEN(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：写使能
*/
static void Flash_WritenEN(void)
{
    sp_w25q64_cfg->cs_select();
    sp_w25q64_cfg->read_one_byte(W25X_WriteEnable);
    sp_w25q64_cfg->cs_unselect();
}

/*
 *  函数名：static void FLASH_WaitForWriteEnd(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：等待写完成
*/
static void FLASH_WaitForWriteEnd(void)
{
    uint8_t flash_status = 0;
    
    sp_w25q64_cfg->cs_select();
    sp_w25q64_cfg->read_one_byte(W25X_ReadStatusReg);

    do
    {
        flash_status = sp_w25q64_cfg->read_one_byte(Dummy_Byte);	 
    }
    while ((flash_status & WIP_Flag) == SET); 
    sp_w25q64_cfg->cs_unselect();
}

/*
 *  函数名：void Flash_PowerDown(void)  
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：断开FLASH
*/
void Flash_PowerDown(void)   
{    
    sp_w25q64_cfg->cs_select();
    
    sp_w25q64_cfg->read_one_byte(W25X_PowerDown);
    
    sp_w25q64_cfg->cs_unselect();
}   

/*
 *  函数名：void Flash_WakeUp(void) 
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：唤醒FLASH
*/
void Flash_WakeUp(void)   
{
    sp_w25q64_cfg->cs_select();
    
    sp_w25q64_cfg->read_one_byte(W25X_ReleasePowerDown);
    
    sp_w25q64_cfg->cs_unselect();
}   

/*
 *  函数名：void FLASH_SectorErase(uint32_t SectorAddr)
 *  输入参数：SectorAddr -> 要擦的扇区地址
 *  输出参数：无
 *  返回值：无
 *  函数作用：扇区擦除
*/
void FLASH_SectorErase(uint32_t SectorAddr)
{
    Flash_WritenEN();
    FLASH_WaitForWriteEnd();

    sp_w25q64_cfg->cs_select();
    sp_w25q64_cfg->read_one_byte(W25X_SectorErase);
    sp_w25q64_cfg->read_one_byte((SectorAddr & 0xFF0000) >> 16);
    sp_w25q64_cfg->read_one_byte((SectorAddr & 0xFF00) >> 8);
    sp_w25q64_cfg->read_one_byte(SectorAddr & 0xFF);
    sp_w25q64_cfg->cs_unselect();
    
    FLASH_WaitForWriteEnd();
}

/*
 *  函数名：void FLASH_BlockErase(uint32_t BlockAddr)
 *  输入参数：BlockAddr -> 要擦擦的块地址
 *  输出参数：无
 *  返回值：无
 *  函数作用：块擦除
*/
void FLASH_BlockErase(uint32_t BlockAddr)
{
    Flash_WritenEN();
    FLASH_WaitForWriteEnd();

    sp_w25q64_cfg->cs_select();
    sp_w25q64_cfg->read_one_byte(W25X_BlockErase);
    sp_w25q64_cfg->read_one_byte((BlockAddr & 0xFF0000) >> 16);
    sp_w25q64_cfg->read_one_byte((BlockAddr & 0xFF00) >> 8);
    sp_w25q64_cfg->read_one_byte(BlockAddr & 0xFF);
    sp_w25q64_cfg->cs_unselect();
    
    FLASH_WaitForWriteEnd();
}
/*
 *  函数名：void FLASH_ChipErase(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：芯片全擦除
*/
void FLASH_ChipErase(void)
{
    Flash_WritenEN();
    
    sp_w25q64_cfg->cs_select();
    sp_w25q64_cfg->read_one_byte(W25X_ChipErase);
    sp_w25q64_cfg->cs_unselect();
    
    FLASH_WaitForWriteEnd();
}

/*
 *  函数名：void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
 *  输入参数：pBuffer -> 要写的数据指针;    WriteAddr -> 要写的FLASH初始地址;  NumByteToWrite -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：页写
*/
void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    Flash_WritenEN();
    
    sp_w25q64_cfg->cs_select();
    sp_w25q64_cfg->read_one_byte(W25X_PageProgram);    
    sp_w25q64_cfg->read_one_byte((WriteAddr & 0xFF0000) >> 16);    
    sp_w25q64_cfg->read_one_byte((WriteAddr & 0xFF00) >> 8);    
    sp_w25q64_cfg->read_one_byte(WriteAddr & 0xFF);
    
    if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
    }

    while (NumByteToWrite--)
    {
        sp_w25q64_cfg->read_one_byte(*pBuffer);
        pBuffer++;
    }
    
    sp_w25q64_cfg->cs_unselect();
    
    FLASH_WaitForWriteEnd();
}

/*
 *  函数名：void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite))
 *  输入参数：pBuffer -> 要写的数据指针;    WriteAddr -> 要写的FLASH初始地址;  NumByteToWrite -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：写N个字节
*/
void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % SPI_FLASH_PageSize;

    count = SPI_FLASH_PageSize - Addr;	

    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;

    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    if (Addr == 0) 
    {
        if (NumOfPage == 0) 
        {
            FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else 
        {
            while (NumOfPage--)
            {
                FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else 
    {
        if (NumOfPage == 0) 
        {
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;

                FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            else 
            {				
                FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else 
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            if (NumOfSingle != 0)
            {
                FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/*
 *  函数名：void FLASH_BufferRead(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite))
 *  输入参数：pBuffer -> 要读的数据指针;    WriteAddr -> 要读的FLASH初始地址;  NumByteToWrite -> 要读的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：读N个字节出来
*/
void FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    sp_w25q64_cfg->cs_select();
    
    sp_w25q64_cfg->read_one_byte(W25X_ReadData);

    sp_w25q64_cfg->read_one_byte((ReadAddr & 0xFF0000) >> 16);
    
    sp_w25q64_cfg->read_one_byte((ReadAddr& 0xFF00) >> 8);
    
    sp_w25q64_cfg->read_one_byte(ReadAddr & 0xFF);

    while (NumByteToRead--)
    {
        *pBuffer = sp_w25q64_cfg->read_one_byte(Dummy_Byte);
        pBuffer++;
    }
    
    sp_w25q64_cfg->cs_unselect();
}

void FLASH_StartReadSequence(uint32_t ReadAddr)
{
    sp_w25q64_cfg->cs_select();
    
    sp_w25q64_cfg->read_one_byte(W25X_ReadData);
    sp_w25q64_cfg->read_one_byte((ReadAddr & 0xFF0000) >> 16);
    sp_w25q64_cfg->read_one_byte((ReadAddr& 0xFF00) >> 8);
    sp_w25q64_cfg->read_one_byte(ReadAddr & 0xFF);
    
    sp_w25q64_cfg->cs_unselect();
}

