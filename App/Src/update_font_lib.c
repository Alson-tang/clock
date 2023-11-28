#include <stdio.h>

#include "uart.h"
#include "common.h"
#include "ymodem.h"
#include "update_font_lib.h"

static COM_StatusTypeDef font_download(void)
{
	uint8_t number[11] = {0};
	uint32_t size = 0;
	COM_StatusTypeDef result;

	Serial_PutString((uint8_t *)"Waiting for file to be sent... (Enter the letter 'A or a' to exit)\n\r");
	result = Ymodem_Receive(&size); // 串口通过Ymodem协议接收数据
	HAL_Delay(10);

	printf("alson-%s, %d, result : %d\r\n", __func__, __LINE__, result);

	if (result == COM_OK)           // 接收正常
	{
		Serial_PutString((uint8_t *)"Download successful\n\r");
		Serial_PutString((uint8_t *)"Download file name: "); 
		Serial_PutString(aFileName);

		Int2Str(number, size);  // int类型转str字符串
		Serial_PutString((uint8_t *)"\n\rSize: ");
		Serial_PutString(number);
		Serial_PutString((uint8_t *)" Bytes\n\r");
	}
	else if (result == COM_LIMIT)
	{
		Serial_PutString((uint8_t *)"\n\r下载程序大小超过FLASH空间!\n\r");
	}
	else if (result == COM_DATA)
	{
		Serial_PutString((uint8_t *)"\n\r数据校验错误!\n\r");
	}
	else if (result == COM_ABORT)
	{
		Serial_PutString((uint8_t *)"\n\r用户中断操作!\n\r");
	}
	else
	{
		Serial_PutString((uint8_t *)"\n\r接收文件失败!\n\r");
	}

	return result;
}

bool update_font_lib(font_lib_type font_type)
{
    __HAL_UART_FLUSH_DRREGISTER(&g_update_font_lib_uart_handle);

    Serial_PutString((uint8_t *)"\n\r");
    if (font_download() == COM_OK) {
        return true;
    } else {
        return false;
    }
}
