################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/examples/libs/sjpg/lv_example_sjpg_1.c 

OBJS += \
./lvgl/examples/libs/sjpg/lv_example_sjpg_1.o 

C_DEPS += \
./lvgl/examples/libs/sjpg/lv_example_sjpg_1.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/examples/libs/sjpg/%.o lvgl/examples/libs/sjpg/%.su lvgl/examples/libs/sjpg/%.cyclo: ../lvgl/examples/libs/sjpg/%.c lvgl/examples/libs/sjpg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../lvgl -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-examples-2f-libs-2f-sjpg

clean-lvgl-2f-examples-2f-libs-2f-sjpg:
	-$(RM) ./lvgl/examples/libs/sjpg/lv_example_sjpg_1.cyclo ./lvgl/examples/libs/sjpg/lv_example_sjpg_1.d ./lvgl/examples/libs/sjpg/lv_example_sjpg_1.o ./lvgl/examples/libs/sjpg/lv_example_sjpg_1.su

.PHONY: clean-lvgl-2f-examples-2f-libs-2f-sjpg
