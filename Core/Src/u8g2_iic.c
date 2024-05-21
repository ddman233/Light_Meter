/*
 * u8g2_iic.c
 *
 *  Created on: 2023Äê7ÔÂ22ÈÕ
 *      Author: 30691
 */
#include<u8g2_iic.h>
#include "main.h"
extern I2C_HandleTypeDef hi2c2;
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg){

	case U8X8_MSG_GPIO_AND_DELAY_INIT:
	    break;

	case U8X8_MSG_DELAY_MILLI:
		HAL_Delay(arg_int);
	    break;

	case U8X8_MSG_GPIO_I2C_CLOCK:
        break;

    case U8X8_MSG_GPIO_I2C_DATA:
        break;

	default:
		return 0;
	}
	return 1; // command processed successfully.
}
uint8_t u8x8_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  static uint8_t buffer[128];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
  static uint8_t buf_idx;
  uint8_t *data;

  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
	buffer[buf_idx++] = *data;
	data++;
	arg_int--;
      }
      break;
    case U8X8_MSG_BYTE_INIT:
      /* add your custom code to init i2c subsystem */
      break;
    case U8X8_MSG_BYTE_SET_DC:
      /* ignored for i2c */
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      buf_idx = 0;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      HAL_I2C_Master_Transmit(&hi2c2,u8x8_GetI2CAddress(u8x8),buffer,buf_idx,1000);
      break;
    default:
      return 0;
  }
  return 1;
}
void u8g2_Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_1(u8g2, U8G2_R0,u8x8_byte_i2c,u8g2_gpio_and_delay_stm32);
   	u8g2_InitDisplay(u8g2);
   	u8g2_SetDisplayRotation(u8g2,U8G2_R2);
   	u8g2_SetPowerSave(u8g2, 0);
	u8g2_ClearBuffer(u8g2);
}
