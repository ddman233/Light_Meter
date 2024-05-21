/*
 * veml7700.h
 *
 *  Created on: Feb 3, 2024
 *      Author: 30691
 */

#ifndef INC_VEML7700_H_
#define INC_VEML7700_H_
	#define ENABLE_VEML7700  1

	#define VEML_SLAVE_ADDR 0x20

	#define COMMAND_ALS_DEFAULT_VALUE       0x01
	// Gain Register
	#define COMMAND_ALS_GAIN                0x00
	#define ALS_GAIN_MASK                   0x1800
	#define ALS_GAIN_SHIFT                  11
	// ALS IT Register
	#define COMMAND_ALS_IT                  0x00
	#define ALS_IT_MASK                     0x03C0
	#define ALS_IT_SHIFT                    6
	// ALS Persistence Register
	#define COMMAND_ALS_PER                 0x00
	#define ALS_PER_MASK                    0x0030
	#define ALS_PER_SHIFT                   4
	// ALS INT EN Register
	#define COMMAND_ALS_INT_EN              0x00
	#define ALS_INT_EN_MASK                 0x0002
	#define ALS_INT_EN_SHIFT                1
	// ALS SD Register
	#define COMMAND_ALS_SD                  0x00
	 #define ALS_SD_MASK                     0x0001
	#define ALS_SD_SHIFT                    0
	// ALS Window High
	#define COMMAND_ALS_WH                  0x01
	// ALS Window Low
	#define COMMAND_ALS_WL                  0x02
	// ALS Power Save Mode
	#define COMMAND_PSM                     0x03
	#define PSM_MASK                        0x0006
	#define PSM_SHIFT                       1
	// ALS Power Save Mode Enable
	#define COMMAND_PSM_EN                  0x03
	#define PSM_EN_MASK                     0x0001
	#define PSM_EN_SHIFT                    0
	// ALS High Resolution Output Data
	#define COMMAND_ALS_DATA                0x04
	//White Channel Output Data
	#define COMMAND_WHITE_DATA              0x05
	// Interrupt Status
	#define COMMAND_ALS_IF_L                0x06
	#define ALS_IF_L_MASK                   0x8000
	#define ALS_IF_L_SHIFT                  15
	#define COMMAND_ALS_IF_H                0x06
	#define ALS_IF_H_MASK                   0x4000
	#define ALS_IF_H_SHIFT                  14

	#define ALS_GAIN_X1                      0x0
	#define ALS_GAIN_X2                      0x1
	#define ALS_GAIN_d8                      0x2
	#define ALS_GAIN_d4                      0x3

	#define ALS_INTEGRATION_25ms             0xC
	#define ALS_INTEGRATION_50ms             0x8
	#define ALS_INTEGRATION_100ms            0x0
	#define ALS_INTEGRATION_200ms            0x1
	#define ALS_INTEGRATION_400ms            0x2
	#define ALS_INTEGRATION_800ms            0x3

	#define ALS_PERSISTENCE_1                0x0
	#define ALS_PERSISTENCE_2                0x1
	#define ALS_PERSISTENCE_4                0x2
	#define ALS_PERSISTENCE_8                0x3

	#define ALS_POWER_MODE_1                 0x0
	#define ALS_POWER_MODE_2                 0x1
	#define ALS_POWER_MODE_3                 0x2
	#define ALS_POWER_MODE_4                 0x3

	#define ALS_POWER_MODE_ENABLE            0x1
	#define ALS_POWER_MODE_DISABLE           0x0

	#define ALS_INTERRUPT_ENABLE             0x1
	#define ALS_INTERRUPT_DISABLE            0x0

	#define ALS_POWER_ON                     0x0
	#define ALS_POWER_SHUTDOWN               0x1

	void veml7700_init(void);
	double get_lux_data(void);
	#endif
