/*
 * veml7700.c
 *
 *  Created on: Feb 3, 2024
 *      Author: 30691
 */


#include "main.h"
#include "veml7700.h"
extern I2C_HandleTypeDef hi2c2;


void veml7700_write(uint8_t reg_addr,uint8_t *p_buf)
{
    HAL_I2C_Mem_Write(&hi2c2,VEML_SLAVE_ADDR,reg_addr,1,p_buf,2,HAL_MAX_DELAY);
}

void veml7700_read(uint8_t reg_addr,uint8_t *p_buf)
{
	HAL_I2C_Mem_Read(&hi2c2,VEML_SLAVE_ADDR,reg_addr,1,p_buf,2,HAL_MAX_DELAY);
}

static void veml7700_set_light_sensor_value(uint8_t value,uint8_t command,uint16_t mask,uint8_t shift)
{
    uint8_t rx_data[2] = {0};
    uint8_t tx_data[2] = {0};
    uint16_t r_value = 0;
    uint16_t tmp_value = 0;
    veml7700_read(command,rx_data);
    r_value = ((rx_data[1] << 8) | rx_data[0]);
    tmp_value = r_value & ~(mask);
    tmp_value |= ((value << shift) & mask);

    tx_data[0] = tmp_value & 0xff;
    tx_data[1] = ((tmp_value >> 8) & 0xff);
    veml7700_write(command,tx_data);
}

static void set_power_enable(int enable)
{
    return veml7700_set_light_sensor_value(enable,COMMAND_ALS_SD,ALS_SD_MASK,ALS_SD_SHIFT);
}

static void set_gain_value(uint16_t gain)
{
    veml7700_set_light_sensor_value(gain,COMMAND_ALS_GAIN,ALS_GAIN_MASK,ALS_GAIN_SHIFT);
}


static void set_interrupt_enable(uint16_t enable)
{
    veml7700_set_light_sensor_value(enable,COMMAND_ALS_INT_EN,ALS_INT_EN_MASK,ALS_INT_EN_SHIFT);
}

static void set_integration_time(int time)
{
    veml7700_set_light_sensor_value(time,COMMAND_ALS_IT,ALS_IT_MASK,ALS_IT_SHIFT);
}

static void set_power_saving_mode(int mode)
{
    veml7700_set_light_sensor_value(mode,COMMAND_PSM_EN,PSM_EN_MASK,PSM_EN_SHIFT);
}

static uint16_t get_als_reg_data()
{
    uint8_t rx_data[2] = {0};
    uint16_t r_value = 0;

    veml7700_read(COMMAND_ALS_DATA,rx_data);
    r_value = ((rx_data[1] << 8) | rx_data[0]);
    return r_value;
}

double get_lux_data(void)
{
    return (get_als_reg_data() * 2.1504);
}


void veml7700_init(void)
{
	//进入设置
    set_power_enable(ALS_POWER_SHUTDOWN);
    //忘记是啥了暑假时候整的
    set_interrupt_enable(ALS_INTERRUPT_DISABLE);
    //决定检测的lux值范围，范围越大分辨率越低
    set_gain_value(ALS_GAIN_d8);
    //积分时间，时间越大分辨率越低，具体gain和积分时间的关系可以看designingveml7700的第5页
    set_integration_time(ALS_INTEGRATION_25ms);
    //省电模式
    set_power_saving_mode(ALS_POWER_MODE_DISABLE);

    set_power_enable(ALS_POWER_ON);
    //退出设置

    //gain = 1/8 IT = 25ms 最大检测lux值 140 926 ，大于1000的需要矫正 例如lux=1500，a x (1500 lux)4 + b x (1500 lux)3 + c x (1500 lux)2 + d x (1500 lux) = 1658 lux
    //    a = 6.0135^-13
    //    b = -9.3924^-9
    //    c = 8.1488^-5
    //    d = 1.0023
}
