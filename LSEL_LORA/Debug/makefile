################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
LSEL_LORA.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
LSEL_LORA.list \

OBJCOPY_BIN += \
LSEL_LORA.bin \


# All Target
all: LSEL_LORA.elf secondary-outputs

# Tool invocations
LSEL_LORA.elf: $(OBJS) $(USER_OBJS) C:\Users\Guillermo\STM32CubeIDE\LSEL_LORA\STM32F411VETX_FLASH.ld
	arm-none-eabi-gcc -o "LSEL_LORA.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Guillermo\STM32CubeIDE\LSEL_LORA\STM32F411VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="LSEL_LORA.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

LSEL_LORA.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "LSEL_LORA.list"
	@echo 'Finished building: $@'
	@echo ' '

LSEL_LORA.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "LSEL_LORA.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
