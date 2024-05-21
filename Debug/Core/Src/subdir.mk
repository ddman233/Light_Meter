################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
../Core/Src/Light_meter.c \
../Core/Src/ee.c \
../Core/Src/main.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c \
../Core/Src/u8g2_iic.c \
../Core/Src/veml7700.c 

OBJS += \
./Core/Src/Light_meter.o \
./Core/Src/ee.o \
./Core/Src/main.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o \
./Core/Src/u8g2_iic.o \
./Core/Src/veml7700.o 

C_DEPS += \
./Core/Src/Light_meter.d \
./Core/Src/ee.d \
./Core/Src/main.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d \
./Core/Src/u8g2_iic.d \
./Core/Src/veml7700.d 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G030xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Light_meter.cyclo ./Core/Src/Light_meter.d ./Core/Src/Light_meter.o ./Core/Src/Light_meter.su ./Core/Src/ee.cyclo ./Core/Src/ee.d ./Core/Src/ee.o ./Core/Src/ee.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32g0xx_hal_msp.cyclo ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.cyclo ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g0xx.cyclo ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su ./Core/Src/u8g2_iic.cyclo ./Core/Src/u8g2_iic.d ./Core/Src/u8g2_iic.o ./Core/Src/u8g2_iic.su ./Core/Src/veml7700.cyclo ./Core/Src/veml7700.d ./Core/Src/veml7700.o ./Core/Src/veml7700.su

.PHONY: clean-Core-2f-Src

