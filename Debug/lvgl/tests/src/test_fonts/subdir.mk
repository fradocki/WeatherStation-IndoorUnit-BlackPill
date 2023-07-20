################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/tests/src/test_fonts/font_1.c \
../lvgl/tests/src/test_fonts/font_2.c \
../lvgl/tests/src/test_fonts/font_3.c 

OBJS += \
./lvgl/tests/src/test_fonts/font_1.o \
./lvgl/tests/src/test_fonts/font_2.o \
./lvgl/tests/src/test_fonts/font_3.o 

C_DEPS += \
./lvgl/tests/src/test_fonts/font_1.d \
./lvgl/tests/src/test_fonts/font_2.d \
./lvgl/tests/src/test_fonts/font_3.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/tests/src/test_fonts/%.o lvgl/tests/src/test_fonts/%.su lvgl/tests/src/test_fonts/%.cyclo: ../lvgl/tests/src/test_fonts/%.c lvgl/tests/src/test_fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../lvgl -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-tests-2f-src-2f-test_fonts

clean-lvgl-2f-tests-2f-src-2f-test_fonts:
	-$(RM) ./lvgl/tests/src/test_fonts/font_1.cyclo ./lvgl/tests/src/test_fonts/font_1.d ./lvgl/tests/src/test_fonts/font_1.o ./lvgl/tests/src/test_fonts/font_1.su ./lvgl/tests/src/test_fonts/font_2.cyclo ./lvgl/tests/src/test_fonts/font_2.d ./lvgl/tests/src/test_fonts/font_2.o ./lvgl/tests/src/test_fonts/font_2.su ./lvgl/tests/src/test_fonts/font_3.cyclo ./lvgl/tests/src/test_fonts/font_3.d ./lvgl/tests/src/test_fonts/font_3.o ./lvgl/tests/src/test_fonts/font_3.su

.PHONY: clean-lvgl-2f-tests-2f-src-2f-test_fonts

