#include "uart.h"
#include "common.h"
 
/*
 *  函数名：void Int2Str(uint8_t *p_str, uint32_t intnum)
 *  输入参数：p_str->指向字符串  intnum->被转化的int
 *  输出参数：无
 *  返回值：无
 *  函数作用：将int类型转化为字符串
*/
void Int2Str(uint8_t *p_str, uint32_t intnum)
{
	uint32_t i, divider = 1000000000, pos = 0, status = 0;

	for (i = 0; i < 10; i++)
	{
		p_str[pos++] = (intnum / divider) + 48;

		intnum = intnum % divider;
		divider /= 10;
		if ((p_str[pos-1] == '0') & (status == 0))
		{
			pos = 0;
		}
		else
		{
			status++;
		}
	}
}

/**
  * @brief  Convert a string to an integer
  * @param  p_inputstr: The string to be converted
  * @param  p_intnum: The integer value
  * @retval 1: Correct
  *         0: Error
  */

/*
 *  函数名：uint32_t Str2Int(uint8_t *p_inputstr, uint32_t *p_intnum)
 *  输入参数：p_inputstr->指向输入字符串  p_intnum->指向转换int
 *  输出参数：无
 *  返回值：1->转化成功 0->转化失败
 *  函数作用：将字符串转化为int类型
*/
uint32_t Str2Int(uint8_t *p_inputstr, uint32_t *p_intnum)
{
	uint32_t i = 0, res = 0;
	uint32_t val = 0;

	if ((p_inputstr[0] == '0') && ((p_inputstr[1] == 'x') || (p_inputstr[1] == 'X')))
	{
		i = 2;
		while ( ( i < 11 ) && ( p_inputstr[i] != '\0' ) )
		{
			if (ISVALIDHEX(p_inputstr[i]))
			{
				val = (val << 4) + CONVERTHEX(p_inputstr[i]);
			}
			else
			{
				/* Return 0, Invalid input */
				res = 0;
				break;
			}
			i++;
		}

		/* valid result */
		if (p_inputstr[i] == '\0')
		{
			*p_intnum = val;
			res = 1;
		}
	}
	else /* max 10-digit decimal input */
	{
		while ( ( i < 11 ) && ( res != 1 ) )
		{
			if (p_inputstr[i] == '\0')
			{
				*p_intnum = val;
				/* return 1 */
				res = 1;
			}
			else if (((p_inputstr[i] == 'k') || (p_inputstr[i] == 'K')) && (i > 0))
			{
				val = val << 10;
				*p_intnum = val;
				res = 1;
			}
			else if (((p_inputstr[i] == 'm') || (p_inputstr[i] == 'M')) && (i > 0))
			{
				val = val << 20;
				*p_intnum = val;
				res = 1;
			}
			else if (ISVALIDDEC(p_inputstr[i]))
			{
				val = val * 10 + CONVERTDEC(p_inputstr[i]);
			}
			else
			{
				/* return 0, Invalid input */
				res = 0;
				break;
			}
			i++;
		}
	}

	return res;
}

/*
 *  函数名：void Serial_PutString(uint8_t *p_string)
 *  输入参数：p_string->打印的字符串
 *  输出参数：无
 *  返回值：无
 *  函数作用：串口打印输出
*/
void Serial_PutString(uint8_t *p_string)
{
	uint16_t length = 0;

	while (p_string[length] != '\0')
	{
		length++;
	}
	HAL_UART_Transmit(p_g_update_font_lib_uart_handle, p_string, length, TX_TIMEOUT);
}
 
/*
 *  函数名：HAL_StatusTypeDef Serial_PutByte( uint8_t param )
 *  输入参数：param->打印的变量
 *  输出参数：无
 *  返回值：HAL_OK->正常传输 HAL_ERROR->错误传输
 *  函数作用：串口打印ASCII变量
*/
HAL_StatusTypeDef Serial_PutByte( uint8_t param )
{
  return HAL_UART_Transmit(p_g_update_font_lib_uart_handle, &param, 1, TX_TIMEOUT);
}
