################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AlfaSDK/Src/alfa_board.c \
../AlfaSDK/Src/alfa_flash.c \
../AlfaSDK/Src/alfa_obc.c \
../AlfaSDK/Src/alfa_rgb.c \
../AlfaSDK/Src/alfa_rtc.c \
../AlfaSDK/Src/alfa_sram.c 

OBJS += \
./AlfaSDK/Src/alfa_board.o \
./AlfaSDK/Src/alfa_flash.o \
./AlfaSDK/Src/alfa_obc.o \
./AlfaSDK/Src/alfa_rgb.o \
./AlfaSDK/Src/alfa_rtc.o \
./AlfaSDK/Src/alfa_sram.o 

C_DEPS += \
./AlfaSDK/Src/alfa_board.d \
./AlfaSDK/Src/alfa_flash.d \
./AlfaSDK/Src/alfa_obc.d \
./AlfaSDK/Src/alfa_rgb.d \
./AlfaSDK/Src/alfa_rtc.d \
./AlfaSDK/Src/alfa_sram.d 


# Each subdirectory must supply rules for building sources it contributes
AlfaSDK/Src/%.o AlfaSDK/Src/%.su AlfaSDK/Src/%.cyclo: ../AlfaSDK/Src/%.c AlfaSDK/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F107xC -c -I../AlfaSDK/Inc -I../App/Inc -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-AlfaSDK-2f-Src

clean-AlfaSDK-2f-Src:
	-$(RM) ./AlfaSDK/Src/alfa_board.cyclo ./AlfaSDK/Src/alfa_board.d ./AlfaSDK/Src/alfa_board.o ./AlfaSDK/Src/alfa_board.su ./AlfaSDK/Src/alfa_flash.cyclo ./AlfaSDK/Src/alfa_flash.d ./AlfaSDK/Src/alfa_flash.o ./AlfaSDK/Src/alfa_flash.su ./AlfaSDK/Src/alfa_obc.cyclo ./AlfaSDK/Src/alfa_obc.d ./AlfaSDK/Src/alfa_obc.o ./AlfaSDK/Src/alfa_obc.su ./AlfaSDK/Src/alfa_rgb.cyclo ./AlfaSDK/Src/alfa_rgb.d ./AlfaSDK/Src/alfa_rgb.o ./AlfaSDK/Src/alfa_rgb.su ./AlfaSDK/Src/alfa_rtc.cyclo ./AlfaSDK/Src/alfa_rtc.d ./AlfaSDK/Src/alfa_rtc.o ./AlfaSDK/Src/alfa_rtc.su ./AlfaSDK/Src/alfa_sram.cyclo ./AlfaSDK/Src/alfa_sram.d ./AlfaSDK/Src/alfa_sram.o ./AlfaSDK/Src/alfa_sram.su

.PHONY: clean-AlfaSDK-2f-Src

