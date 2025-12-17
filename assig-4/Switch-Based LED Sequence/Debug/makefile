################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Switch-Based LED Sequence
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Switch-Based\ LED\ Sequence.elf \

MAP_FILES += \
Switch-Based\ LED\ Sequence.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Switch-Based\ LED\ Sequence.list \


# All Target
all: main-build

# Main-build Target
main-build: Switch-Based\ LED\ Sequence.elf secondary-outputs

# Tool invocations
Switch-Based\ LED\ Sequence.elf Switch-Based\ LED\ Sequence.map: $(OBJS) $(USER_OBJS) C:\Users\Omkar\OneDrive\Desktop\Sunbeam\ARM-94112-omkar(repo)\ARM_94112_Omkar_Kore\STM32\Switch-Based\ LED\ Sequence\STM32F407VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Switch-Based LED Sequence.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Omkar\OneDrive\Desktop\Sunbeam\ARM-94112-omkar(repo)\ARM_94112_Omkar_Kore\STM32\Switch-Based LED Sequence\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Switch-Based LED Sequence.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Switch-Based\ LED\ Sequence.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Switch-Based LED Sequence.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Switch-Based\ LED\ Sequence.elf Switch-Based\ LED\ Sequence.list Switch-Based\ LED\ Sequence.map Switch-Based\\ LED\\ Sequence.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
