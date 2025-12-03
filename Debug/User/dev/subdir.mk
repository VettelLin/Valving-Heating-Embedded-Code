################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/dev/WS2812B.c 

OBJS += \
./User/dev/WS2812B.o 

C_DEPS += \
./User/dev/WS2812B.d 


# Each subdirectory must supply rules for building sources it contributes
User/dev/%.o User/dev/%.su User/dev/%.cyclo: ../User/dev/%.c User/dev/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/ra18_dock/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-dev

clean-User-2f-dev:
	-$(RM) ./User/dev/WS2812B.cyclo ./User/dev/WS2812B.d ./User/dev/WS2812B.o ./User/dev/WS2812B.su

.PHONY: clean-User-2f-dev

