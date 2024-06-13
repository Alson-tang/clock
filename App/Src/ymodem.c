#include <stdio.h>
#include "common.h"
#include "string.h"
#include "main.h"
#include "uart.h"
#include "w25q64.h"
#include "ymodem.h"

// 定义数据包,需保持32为对齐
uint8_t aPacketData[PACKET_1K_SIZE + PACKET_DATA_INDEX + PACKET_TRAILER_SIZE]; 

static HAL_StatusTypeDef ReceivePacket(uint8_t *p_data, uint32_t *p_length, uint32_t timeout);
uint16_t UpdateCRC16(uint16_t crc_in, uint8_t byte);
uint16_t Cal_CRC16(const uint8_t* p_data, uint32_t size);
uint8_t CalcChecksum(const uint8_t *p_data, uint32_t size);

uint8_t aFileName[FILE_NAME_LENGTH];
uint8_t start_addr[FILE_START_ADDR_LENGTH] = { 0 };

/*
 *  函数名：static HAL_StatusTypeDef ReceivePacket(uint8_t *p_data, uint32_t *p_length, uint32_t timeout)
 *  输入参数：timeout->重新接收超时时间
 *  输出参数：p_data->指向接收的数据 p_length->指向接收数据长度
 *  返回值：HAL_OK->正常返回 HAL_BUSY->被用户中止
 *  函数作用：接收来自发送者的数据包
*/
static HAL_StatusTypeDef ReceivePacket(uint8_t *p_data, uint32_t *p_length, uint32_t timeout)
{
    uint32_t recv_crc;
    uint32_t cal_crc;
    uint32_t packet_size = 0;
    HAL_StatusTypeDef status;
    uint8_t char1;

    *p_length = 0;
    status = HAL_UART_Receive(p_g_update_font_lib_uart_handle, &char1, 1, timeout);  // 接收一个字节数据

    if (status == HAL_OK)
    {
        switch (char1)                        // 判断接收的内容
        {
            case SOH:                         // SOH表示本数据包大小为128字节
                packet_size = PACKET_SIZE;    // 定义包大小128字节
                break;
            case STX:                         // STX表示本数据包大小为1K字节
                packet_size = PACKET_1K_SIZE; // 定义包大小1K
                break;
            case EOT:                         // EOT表示传输完成
                break;
            case CA:                          // 连续收到两个CA(Cancel)表示取消传输
                if ((HAL_UART_Receive(p_g_update_font_lib_uart_handle, &char1, 1, timeout) == HAL_OK) && (char1 == CA)) //再接收到一个CA
                    packet_size = 2;          // 定义包大小2字节
                else
                    status = HAL_ERROR;
                break;
            case ABORT1:                       // ABORT表示中止(收到A/a),返回HAL_BUSY      
            case ABORT2:
                status = HAL_BUSY;
                break;
            default:
                status = HAL_ERROR;
                break;
        }

        *p_data = char1;                 // 
        // printf("alson-%s, %d, packet_size : %d\r\n", __func__, __LINE__, packet_size);

        if (packet_size >= PACKET_SIZE) // 如果数据包>=128字节,即接收数据包
        {
            // printf("alson-%s, %d\r\n", __func__, __LINE__);
            // 串口接收数据(保存位置:p_data[2]开始  保存长度:数据长度+数据包其它信息-数据包起始信号
            status = HAL_UART_Receive(p_g_update_font_lib_uart_handle, &p_data[PACKET_NUMBER_INDEX], packet_size + PACKET_OVERHEAD_SIZE - 1, timeout);
            // printf("alson-%s, %d, status : %d, len is : %d\r\n", __func__, __LINE__, status, packet_size + PACKET_OVERHEAD_SIZE - 1);

            if (status == HAL_OK) // 简单的检查数据包完整性
            {
                if (p_data[PACKET_NUMBER_INDEX] != ((p_data[PACKET_CNUMBER_INDEX]) ^ NEGATIVE_BYTE)) // 反码校验
                {
                    packet_size = 0;
                    status = HAL_ERROR;
                }
                else  // CRC校验
                {
                    recv_crc = 0;
                    cal_crc = recv_crc;

                    recv_crc = p_data[packet_size + PACKET_DATA_INDEX] << 8; // CRC高8位
                    recv_crc += p_data[packet_size + PACKET_DATA_INDEX + 1]; // CRC低8位

                    cal_crc = Cal_CRC16(&p_data[PACKET_DATA_INDEX], packet_size);

                    if (cal_crc !=  recv_crc)
                    {
                        packet_size = 0;
                        status = HAL_ERROR;
                    }
                }
            }
            else
            {
                packet_size = 0;
            }
        }
    }

    *p_length = packet_size;  // 返回包大小

    return status;
}

/**
  * @brief  Update CRC16 for input byte
  * @param  crc_in input value 
  * @param  input byte
  * @retval None
  */

/*
 *  函数名：uint16_t UpdateCRC16(uint16_t crc_in, uint8_t byte)
 *  输入参数：crc_in->crc输入 byte->字节数据
 *  输出参数：无
 *  返回值：crc->新crc
 *  函数作用：更新CRC16的输入字节
*/
uint16_t UpdateCRC16(uint16_t crc_in, uint8_t byte)
{
    uint32_t crc = crc_in;
    uint32_t in = byte | 0x100;

    do
    {
    crc <<= 1;
    in <<= 1;
    if(in & 0x100)
        ++crc;
    if(crc & 0x10000)
        crc ^= 0x1021;
    }

    while(!(in & 0x10000));

    return crc & 0xffffu;
}

/*
 *  函数名：uint16_t Cal_CRC16(const uint8_t* p_data, uint32_t size)
 *  输入参数：data->校验数据 size->数据长度
 *  输出参数：无
 *  返回值：CRC计算结果
 *  函数作用：对YModem数据包进行CRC16计算
*/
uint16_t Cal_CRC16(const uint8_t* p_data, uint32_t size)
{
    uint32_t crc = 0;
    const uint8_t* dataEnd = p_data+size;

    while(p_data < dataEnd)
    crc = UpdateCRC16(crc, *p_data++);

    crc = UpdateCRC16(crc, 0);
    crc = UpdateCRC16(crc, 0);

    return crc&0xffffu;
}

/*
 *  函数名：COM_StatusTypeDef Ymodem_Receive (uint32_t *p_size)
 *  输入参数：无
 *  输出参数：接收文件大小
 *  返回值：接收/下载结果
 *  函数作用：使用带有CRC16校验的Ymodem协议接收文件
*/
COM_StatusTypeDef Ymodem_Receive(uint32_t *p_size)
{
    uint32_t i, packet_length, session_done = 0, file_done, errors = 0, session_begin = 0;
    uint32_t flashdestination, ramsource, filesize, packets_received;
    uint8_t *file_ptr;
    uint8_t file_size[FILE_SIZE_LENGTH], tmp;
    uint8_t *rec_buf = NULL;
    COM_StatusTypeDef result = COM_OK;
    uint8_t eot_received = 0;
    uint32_t block_num = 0;

    while ((session_done == 0) && (result == COM_OK))   // 直至传输会话完成,且COM口状态正常
    {
        packets_received = 0;                           // 统计收到的数据包个数
        file_done = 0;
        while ((file_done == 0) && (result == COM_OK))  // 直至文件传输完成,且COM口状态正常
        {
            switch (ReceivePacket(aPacketData, &packet_length, DOWNLOAD_TIMEOUT)) // 接收数据包
            {
                case HAL_OK:                     //正常接收
                    errors = 0;
                    // printf("alson-%s, %d, packet_length : %d\r\n", __func__, __LINE__, packet_length);
                    switch (packet_length)       // 根据数据长度对应处理
                    {
                        case 2:                  // 发送方中止
                            Serial_PutByte(ACK); // 发送ACK
                            result = COM_ABORT;
                            break;
                        case 0:                  // 正常传输结束
                            eot_received++;
                            if (eot_received == 1) {
                                Serial_PutByte(NAK); // 发送NAK信号
                            } else {
                                eot_received = 0;
                                Serial_PutByte(ACK); // 发送ACK
                            }
                            file_done = 1;
                            break;
                        default:                 // 正常数据包
                            // printf("alson-%s, %d\r\n", __func__, __LINE__);
                            if (aPacketData[PACKET_NUMBER_INDEX] != (packets_received & 0xFF) )   // 数据包编号和接收数量序号不一致
                            {
                                // printf("alson-%s, %d, index : %d, rec index : %d\r\n", __func__, __LINE__, aPacketData[PACKET_NUMBER_INDEX], packets_received);
                                Serial_PutByte(NAK); // 发送NAK信号
                            }
                            else
                            {
                                // printf("alson-%s, %d\r\n", __func__, __LINE__);
                                if (packets_received == 0)  // 第一个数据包,为起始帧,包含文件信息
                                {
                                    
                                    if (aPacketData[PACKET_DATA_INDEX] != 0)  // 数据包数据不为空
                                    {
                                        /* File name */
                                        i = 0;
                                        file_ptr = aPacketData + PACKET_DATA_INDEX;  
                                        while ( (*file_ptr != NULL) && (i < FILE_NAME_LENGTH))
                                        {
                                            aFileName[i++] = *file_ptr++;
                                        }
                                        
                                        /* File size */
                                        aFileName[i++] = '\0';
                                        i = 0;
                                        file_ptr++;
                                        while ( (*file_ptr != NULL) && (i < FILE_SIZE_LENGTH))
                                        {
                                            file_size[i++] = *file_ptr++;
                                        }
                                        file_size[i++] = '\0';
                                        Str2Int(file_size, &filesize); // 字符串转int类型

                                        /* Start address */
                                        i = 0;
                                        file_ptr++;
                                        while ( (*file_ptr != NULL) && (i < FILE_START_ADDR_LENGTH)) {
                                            start_addr[i++] = *file_ptr++;
                                        }
                                        start_addr[i++] = '\0';
                                        Str2Int(start_addr, &flashdestination);

                                        // printf("alson-%s, %d, name:%s\r\n", __func__, __LINE__, aFileName);
                                        // printf("alson-%s, %d, size:%d\r\n", __func__, __LINE__, filesize);
                                        // printf("alson-%s, %d, addr:%d\r\n", __func__, __LINE__, flashdestination);

                                        // 测试要发送的image是否大于FLASH大小
                                        // if (*p_size > (USER_FLASH_SIZE + 1))
                                        // {
                                        // 	// 超出限制,结束会话
                                        // 	tmp = CA;
                                        // 	HAL_UART_Transmit(p_g_update_font_lib_uart_handle, &tmp, 1, NAK_TIMEOUT);
                                        // 	HAL_UART_Transmit(p_g_update_font_lib_uart_handle, &tmp, 1, NAK_TIMEOUT);
                                        // 	result = COM_LIMIT;
                                        // }

                                        /* Erase the corresponding area in FLASH */
                                        block_num = filesize / SPI_FLASH_BLOCK_SIZE;
                                        if (filesize & (~SPI_FLASH_BLOCK_SIZE)) {
                                            block_num += 1;
                                        }
                                        // printf("alson-%s, %d, block_num : %d\r\n", __func__, __LINE__, block_num);
                                        for (uint32_t i = 0; i < block_num; i++) {
                                            FLASH_BlockErase(flashdestination + (i * SPI_FLASH_BLOCK_SIZE));
                                        }

                                        *p_size = filesize;

                                        Serial_PutByte(ACK);
                                        Serial_PutByte(CRC16);
                                    }
                                    else   // 文件头数据包为空，结束会话
                                    {
                                        Serial_PutByte(ACK);
                                        file_done = 1;
                                        session_done = 1;
                                        break;
                                    }
                                }
                                else
                                {
                                    ramsource = (uint32_t) & aPacketData[PACKET_DATA_INDEX]; // 得到数据包数据
                                    rec_buf = (uint8_t *) & aPacketData[PACKET_DATA_INDEX];

                                    // 将接收的数据写入FLASH  
                                    // if (FLASH_If_Write(flashdestination, (uint32_t*) ramsource, packet_length/4) == FLASHIF_OK)
                                    // {
                                    // 	flashdestination += packet_length;
                                    // 	Serial_PutByte(ACK);
                                    // }
                                    // else // 写入FLASH出错
                                    // {	
                                    // 	// 数据错误,结束会话
                                    // 	Serial_PutByte(CA);
                                    // 	Serial_PutByte(CA);
                                    // 	result = COM_DATA;
                                    // }

                                    // printf("flashdestination is : %08X, packet_length : %d\r\n", flashdestination, packet_length);
                                    FLASH_BufferWrite(rec_buf, flashdestination, packet_length);

                                    flashdestination += packet_length;
                                    Serial_PutByte(ACK);
                                }
                                packets_received ++; // 接收数据包数量加1
                                session_begin = 1;   // 会话开始标记
                            }
                            break;
                    }
                    break;
                case HAL_BUSY:                  // 中止传输
                    Serial_PutByte(CA);
                    Serial_PutByte(CA);
                    result = COM_ABORT;
                    break;
                default:
                    if (session_begin > 0)
                    {
                        errors ++;
                    }
                    
                    if (errors > MAX_ERRORS)    // 中止通信
                    {
                        Serial_PutByte(CA);
                        Serial_PutByte(CA);
                    }
                    else
                    {
                        Serial_PutByte(CRC16);  // 发送字符C,请求数据包
                    }
                    break;
            }
        }
    }
    return result;
}
