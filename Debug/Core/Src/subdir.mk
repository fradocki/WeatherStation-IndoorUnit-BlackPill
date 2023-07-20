################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/fatfs_sd.c \
../Core/Src/fr_ESP8266.c \
../Core/Src/fr_ESP8266_messages.c \
../Core/Src/fr_battery.c \
../Core/Src/fr_dht22.c \
../Core/Src/fr_eeprom.c \
../Core/Src/fr_global.c \
../Core/Src/fr_lcd.c \
../Core/Src/fr_lcd_lvgl_touch.c \
../Core/Src/fr_lv_drv_touch.c \
../Core/Src/fr_scd40.c \
../Core/Src/fr_sd_handle.c \
../Core/Src/fr_sd_logger.c \
../Core/Src/fr_stm32.c \
../Core/Src/fr_touch.c \
../Core/Src/fr_ui_events.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/ui.c \
../Core/Src/ui_helpers.c \
../Core/Src/ui_img_313407983.c 

OBJS += \
./Core/Src/fatfs_sd.o \
./Core/Src/fr_ESP8266.o \
./Core/Src/fr_ESP8266_messages.o \
./Core/Src/fr_battery.o \
./Core/Src/fr_dht22.o \
./Core/Src/fr_eeprom.o \
./Core/Src/fr_global.o \
./Core/Src/fr_lcd.o \
./Core/Src/fr_lcd_lvgl_touch.o \
./Core/Src/fr_lv_drv_touch.o \
./Core/Src/fr_scd40.o \
./Core/Src/fr_sd_handle.o \
./Core/Src/fr_sd_logger.o \
./Core/Src/fr_stm32.o \
./Core/Src/fr_touch.o \
./Core/Src/fr_ui_events.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/ui.o \
./Core/Src/ui_helpers.o \
./Core/Src/ui_img_313407983.o 

C_DEPS += \
./Core/Src/fatfs_sd.d \
./Core/Src/fr_ESP8266.d \
./Core/Src/fr_ESP8266_messages.d \
./Core/Src/fr_battery.d \
./Core/Src/fr_dht22.d \
./Core/Src/fr_eeprom.d \
./Core/Src/fr_global.d \
./Core/Src/fr_lcd.d \
./Core/Src/fr_lcd_lvgl_touch.d \
./Core/Src/fr_lv_drv_touch.d \
./Core/Src/fr_scd40.d \
./Core/Src/fr_sd_handle.d \
./Core/Src/fr_sd_logger.d \
./Core/Src/fr_stm32.d \
./Core/Src/fr_touch.d \
./Core/Src/fr_ui_events.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/ui.d \
./Core/Src/ui_helpers.d \
./Core/Src/ui_img_313407983.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../lvgl -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/fatfs_sd.cyclo ./Core/Src/fatfs_sd.d ./Core/Src/fatfs_sd.o ./Core/Src/fatfs_sd.su ./Core/Src/fr_ESP8266.cyclo ./Core/Src/fr_ESP8266.d ./Core/Src/fr_ESP8266.o ./Core/Src/fr_ESP8266.su ./Core/Src/fr_ESP8266_messages.cyclo ./Core/Src/fr_ESP8266_messages.d ./Core/Src/fr_ESP8266_messages.o ./Core/Src/fr_ESP8266_messages.su ./Core/Src/fr_battery.cyclo ./Core/Src/fr_battery.d ./Core/Src/fr_battery.o ./Core/Src/fr_battery.su ./Core/Src/fr_dht22.cyclo ./Core/Src/fr_dht22.d ./Core/Src/fr_dht22.o ./Core/Src/fr_dht22.su ./Core/Src/fr_eeprom.cyclo ./Core/Src/fr_eeprom.d ./Core/Src/fr_eeprom.o ./Core/Src/fr_eeprom.su ./Core/Src/fr_global.cyclo ./Core/Src/fr_global.d ./Core/Src/fr_global.o ./Core/Src/fr_global.su ./Core/Src/fr_lcd.cyclo ./Core/Src/fr_lcd.d ./Core/Src/fr_lcd.o ./Core/Src/fr_lcd.su ./Core/Src/fr_lcd_lvgl_touch.cyclo ./Core/Src/fr_lcd_lvgl_touch.d ./Core/Src/fr_lcd_lvgl_touch.o ./Core/Src/fr_lcd_lvgl_touch.su ./Core/Src/fr_lv_drv_touch.cyclo ./Core/Src/fr_lv_drv_touch.d ./Core/Src/fr_lv_drv_touch.o ./Core/Src/fr_lv_drv_touch.su ./Core/Src/fr_scd40.cyclo ./Core/Src/fr_scd40.d ./Core/Src/fr_scd40.o ./Core/Src/fr_scd40.su ./Core/Src/fr_sd_handle.cyclo ./Core/Src/fr_sd_handle.d ./Core/Src/fr_sd_handle.o ./Core/Src/fr_sd_handle.su ./Core/Src/fr_sd_logger.cyclo ./Core/Src/fr_sd_logger.d ./Core/Src/fr_sd_logger.o ./Core/Src/fr_sd_logger.su ./Core/Src/fr_stm32.cyclo ./Core/Src/fr_stm32.d ./Core/Src/fr_stm32.o ./Core/Src/fr_stm32.su ./Core/Src/fr_touch.cyclo ./Core/Src/fr_touch.d ./Core/Src/fr_touch.o ./Core/Src/fr_touch.su ./Core/Src/fr_ui_events.cyclo ./Core/Src/fr_ui_events.d ./Core/Src/fr_ui_events.o ./Core/Src/fr_ui_events.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/ui.cyclo ./Core/Src/ui.d ./Core/Src/ui.o ./Core/Src/ui.su ./Core/Src/ui_helpers.cyclo ./Core/Src/ui_helpers.d ./Core/Src/ui_helpers.o ./Core/Src/ui_helpers.su ./Core/Src/ui_img_313407983.cyclo ./Core/Src/ui_img_313407983.d ./Core/Src/ui_img_313407983.o ./Core/Src/ui_img_313407983.su

.PHONY: clean-Core-2f-Src

