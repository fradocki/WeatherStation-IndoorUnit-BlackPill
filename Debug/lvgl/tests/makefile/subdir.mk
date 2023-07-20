################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/tests/makefile/test.c 

OBJS += \
./lvgl/tests/makefile/test.o 

C_DEPS += \
./lvgl/tests/makefile/test.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/tests/makefile/%.o lvgl/tests/makefile/%.su lvgl/tests/makefile/%.cyclo: ../lvgl/tests/makefile/%.c lvgl/tests/makefile/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../lvgl -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-tests-2f-makefile

clean-lvgl-2f-tests-2f-makefile:
	-$(RM) ./lvgl/tests/makefile/test.cyclo ./lvgl/tests/makefile/test.d ./lvgl/tests/makefile/test.o ./lvgl/tests/makefile/test.su

.PHONY: clean-lvgl-2f-tests-2f-makefile

