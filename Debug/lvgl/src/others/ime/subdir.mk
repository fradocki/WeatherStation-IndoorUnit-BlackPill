################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/others/ime/lv_ime_pinyin.c 

OBJS += \
./lvgl/src/others/ime/lv_ime_pinyin.o 

C_DEPS += \
./lvgl/src/others/ime/lv_ime_pinyin.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/others/ime/%.o lvgl/src/others/ime/%.su lvgl/src/others/ime/%.cyclo: ../lvgl/src/others/ime/%.c lvgl/src/others/ime/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../lvgl -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-others-2f-ime

clean-lvgl-2f-src-2f-others-2f-ime:
	-$(RM) ./lvgl/src/others/ime/lv_ime_pinyin.cyclo ./lvgl/src/others/ime/lv_ime_pinyin.d ./lvgl/src/others/ime/lv_ime_pinyin.o ./lvgl/src/others/ime/lv_ime_pinyin.su

.PHONY: clean-lvgl-2f-src-2f-others-2f-ime

