################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/userInterface/displayDriver.c \
../src/userInterface/qTouch.c \
../src/userInterface/realTimeClock.c 

OBJS += \
./src/userInterface/displayDriver.o \
./src/userInterface/qTouch.o \
./src/userInterface/realTimeClock.o 

C_DEPS += \
./src/userInterface/displayDriver.d \
./src/userInterface/qTouch.d \
./src/userInterface/realTimeClock.d 


# Each subdirectory must supply rules for building sources it contributes
src/userInterface/%.o: ../src/userInterface/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I../src -I../FreeRTOS_include -I../FreeRTOS_portable -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


