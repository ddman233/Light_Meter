/*
 * u8g2_iic.h
 *
 *  Created on: 2023Äê7ÔÂ22ÈÕ
 *      Author: 30691
 */

#ifndef INC_U8G2_IIC_H_
#define INC_U8G2_IIC_H_
#include "csrc/u8g2.h"
#include "main.h"
#endif /* INC_U8G2_IIC_H_ */
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2_Init(u8g2_t *u8g2);
