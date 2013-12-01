################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sensorsMenagement/DS18B20.c \
../src/sensorsMenagement/SHT21.c \
../src/sensorsMenagement/sensorControl.c 

OBJS += \
./src/sensorsMenagement/DS18B20.o \
./src/sensorsMenagement/SHT21.o \
./src/sensorsMenagement/sensorControl.o 

C_DEPS += \
./src/sensorsMenagement/DS18B20.d \
./src/sensorsMenagement/SHT21.d \
./src/sensorsMenagement/sensorControl.d 


# Each subdirectory must supply rules for building sources it contributes
src/sensorsMenagement/%.o: ../src/sensorsMenagement/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I../src -I../FreeRTOS_include -I../FreeRTOS_portable -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


