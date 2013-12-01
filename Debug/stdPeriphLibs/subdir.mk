################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stdPeriphLibs/core_cm3.c \
../stdPeriphLibs/lpc17xx_adc.c \
../stdPeriphLibs/lpc17xx_clkpwr.c \
../stdPeriphLibs/lpc17xx_dac.c \
../stdPeriphLibs/lpc17xx_exti.c \
../stdPeriphLibs/lpc17xx_gpdma.c \
../stdPeriphLibs/lpc17xx_gpio.c \
../stdPeriphLibs/lpc17xx_i2c.c \
../stdPeriphLibs/lpc17xx_i2s.c \
../stdPeriphLibs/lpc17xx_libcfg_default.c \
../stdPeriphLibs/lpc17xx_nvic.c \
../stdPeriphLibs/lpc17xx_pinsel.c \
../stdPeriphLibs/lpc17xx_pwm.c \
../stdPeriphLibs/lpc17xx_rit.c \
../stdPeriphLibs/lpc17xx_rtc.c \
../stdPeriphLibs/lpc17xx_spi.c \
../stdPeriphLibs/lpc17xx_ssp.c \
../stdPeriphLibs/lpc17xx_systick.c \
../stdPeriphLibs/lpc17xx_timer.c \
../stdPeriphLibs/lpc17xx_uart.c \
../stdPeriphLibs/lpc17xx_wdt.c \
../stdPeriphLibs/system_LPC17xx.c 

OBJS += \
./stdPeriphLibs/core_cm3.o \
./stdPeriphLibs/lpc17xx_adc.o \
./stdPeriphLibs/lpc17xx_clkpwr.o \
./stdPeriphLibs/lpc17xx_dac.o \
./stdPeriphLibs/lpc17xx_exti.o \
./stdPeriphLibs/lpc17xx_gpdma.o \
./stdPeriphLibs/lpc17xx_gpio.o \
./stdPeriphLibs/lpc17xx_i2c.o \
./stdPeriphLibs/lpc17xx_i2s.o \
./stdPeriphLibs/lpc17xx_libcfg_default.o \
./stdPeriphLibs/lpc17xx_nvic.o \
./stdPeriphLibs/lpc17xx_pinsel.o \
./stdPeriphLibs/lpc17xx_pwm.o \
./stdPeriphLibs/lpc17xx_rit.o \
./stdPeriphLibs/lpc17xx_rtc.o \
./stdPeriphLibs/lpc17xx_spi.o \
./stdPeriphLibs/lpc17xx_ssp.o \
./stdPeriphLibs/lpc17xx_systick.o \
./stdPeriphLibs/lpc17xx_timer.o \
./stdPeriphLibs/lpc17xx_uart.o \
./stdPeriphLibs/lpc17xx_wdt.o \
./stdPeriphLibs/system_LPC17xx.o 

C_DEPS += \
./stdPeriphLibs/core_cm3.d \
./stdPeriphLibs/lpc17xx_adc.d \
./stdPeriphLibs/lpc17xx_clkpwr.d \
./stdPeriphLibs/lpc17xx_dac.d \
./stdPeriphLibs/lpc17xx_exti.d \
./stdPeriphLibs/lpc17xx_gpdma.d \
./stdPeriphLibs/lpc17xx_gpio.d \
./stdPeriphLibs/lpc17xx_i2c.d \
./stdPeriphLibs/lpc17xx_i2s.d \
./stdPeriphLibs/lpc17xx_libcfg_default.d \
./stdPeriphLibs/lpc17xx_nvic.d \
./stdPeriphLibs/lpc17xx_pinsel.d \
./stdPeriphLibs/lpc17xx_pwm.d \
./stdPeriphLibs/lpc17xx_rit.d \
./stdPeriphLibs/lpc17xx_rtc.d \
./stdPeriphLibs/lpc17xx_spi.d \
./stdPeriphLibs/lpc17xx_ssp.d \
./stdPeriphLibs/lpc17xx_systick.d \
./stdPeriphLibs/lpc17xx_timer.d \
./stdPeriphLibs/lpc17xx_uart.d \
./stdPeriphLibs/lpc17xx_wdt.d \
./stdPeriphLibs/system_LPC17xx.d 


# Each subdirectory must supply rules for building sources it contributes
stdPeriphLibs/%.o: ../stdPeriphLibs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I../src -I../FreeRTOS_include -I../FreeRTOS_portable -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


