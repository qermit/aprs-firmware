################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include stdPeriphLibs/subdir.mk
-include src/userInterface/subdir.mk
-include src/tasks/subdir.mk
-include src/sensorsMenagement/subdir.mk
-include src/powerManagement/subdir.mk
-include src/gsmModule/subdir.mk
-include src/gpsModule/subdir.mk
-include src/subdir.mk
-include FreeRTOS_src/subdir.mk
-include FreeRTOS_portable/MemMang/subdir.mk
-include FreeRTOS_portable/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: sensorBox.axf

# Tool invocations
sensorBox.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="sensorBox.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "sensorBox_Debug.ld" -o "sensorBox.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) sensorBox.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "sensorBox.axf"; # arm-none-eabi-objcopy -O binary "sensorBox.axf" "sensorBox.bin" ; checksum -p LPC1754 -d "sensorBox.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
