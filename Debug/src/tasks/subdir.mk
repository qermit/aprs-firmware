################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/tasks/idleTask.c \
../src/tasks/measurementsTask.c \
../src/tasks/taskGpsLocalization.c \
../src/tasks/taskLedBlinking.c \
../src/tasks/userTask.c 

OBJS += \
./src/tasks/idleTask.o \
./src/tasks/measurementsTask.o \
./src/tasks/taskGpsLocalization.o \
./src/tasks/taskLedBlinking.o \
./src/tasks/userTask.o 

C_DEPS += \
./src/tasks/idleTask.d \
./src/tasks/measurementsTask.d \
./src/tasks/taskGpsLocalization.d \
./src/tasks/taskLedBlinking.d \
./src/tasks/userTask.d 


# Each subdirectory must supply rules for building sources it contributes
src/tasks/%.o: ../src/tasks/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I../src -I../FreeRTOS_include -I../FreeRTOS_portable -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


