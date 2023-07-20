################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/env_support/rt-thread/squareline/ui/lv_ui_entry.c 

OBJS += \
./lvgl/env_support/rt-thread/squareline/ui/lv_ui_entry.o 

C_DEPS += \
./lvgl/env_support/rt-thread/squareline/ui/lv_ui_entry.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/env_support/rt-thread/squareline/ui/%.o lvgl/env_support/rt-thread/squareline/ui/%.su lvgl/env_support/rt-thread/squareline/ui/%.cyclo: ../lvgl/env_support/rt-thread/squareline/ui/%.c lvgl/env_support/rt-thread/squareline/ui/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../lvgl -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-env_support-2f-rt-2d-thread-2f-squareline-2f-ui

clean-lvgl-2f-env_support-2f-rt-2d-thread-2f-squareline-2f-ui:
	-$(RM) ./lvgl/env_support/rt-thread/squareline/ui/lv_ui_entry.cyclo ./lvgl/env_support/rt-thread/squareline/ui/lv_ui_entry.d ./lvgl/env_support/rt-thread/squareline/ui/lv_ui_entry.o ./lvgl/env_support/rt-thread/squareline/ui/lv_ui_entry.su

.PHONY: clean-lvgl-2f-env_support-2f-rt-2d-thread-2f-squareline-2f-ui

