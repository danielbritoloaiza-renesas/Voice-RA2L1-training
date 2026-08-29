################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Util/subdir.mk
-include src/Speex/subdir.mk
-include src/SEGGER_RTT/subdir.mk
-include src/Model/subdir.mk
-include src/DSpotter/subdir.mk
-include src/subdir.mk
-include ra_gen/subdir.mk
-include ra/fsp/src/r_spi/subdir.mk
-include ra/fsp/src/r_sci_uart/subdir.mk
-include ra/fsp/src/r_poeg/subdir.mk
-include ra/fsp/src/r_lpm/subdir.mk
-include ra/fsp/src/r_ioport/subdir.mk
-include ra/fsp/src/r_gpt/subdir.mk
-include ra/fsp/src/r_flash_lp/subdir.mk
-include ra/fsp/src/r_elc/subdir.mk
-include ra/fsp/src/r_dtc/subdir.mk
-include ra/fsp/src/r_dac/subdir.mk
-include ra/fsp/src/r_agt/subdir.mk
-include ra/fsp/src/r_adc/subdir.mk
-include ra/fsp/src/bsp/mcu/ra2l1/subdir.mk
-include ra/fsp/src/bsp/mcu/all/subdir.mk
-include ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/subdir.mk
-include ra/board/ra2l1_voice/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
-include $(P_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
-include $(PP_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
-include $(FSY_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
-include $(JMP_UPPER_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ra2l1_voice_vc_ds22x.srec \

SECONDARY_SIZE += \
ra2l1_voice_vc_ds22x.siz \


# All Target
all: ra2l1_voice_vc_ds22x.elf secondary-outputs

# Tool invocations
ra2l1_voice_vc_ds22x.elf: $(OBJS) $(USER_OBJS) $(LINKER_SCRIPT)
	$(file > ra2l1_voice_vc_ds22x.elf.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -T "fsp.ld" -Xlinker --gc-sections -L "C:\\Users\\a5166677\\e2_studio\\workspace\\ra2l1_voice_vc_ds22x\\ra2l1_voice_vc_ds22x/script" -L "C:\\Users\\a5166677\\e2_studio\\workspace\\ra2l1_voice_vc_ds22x\\ra2l1_voice_vc_ds22x\\src\\DSpotter\\lib" -L "C:\\Users\\a5166677\\e2_studio\\workspace\\ra2l1_voice_vc_ds22x\\ra2l1_voice_vc_ds22x\\src\\Speex\\lib" -Wl,-Map,"ra2l1_voice_vc_ds22x.map" --specs=nano.specs --specs=rdimon.specs -o "ra2l1_voice_vc_ds22x.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group)
	@echo Building target: $@ && arm-none-eabi-gcc @"ra2l1_voice_vc_ds22x.elf.in"
ra2l1_voice_vc_ds22x.srec: ra2l1_voice_vc_ds22x.elf
	arm-none-eabi-objcopy -O srec "ra2l1_voice_vc_ds22x.elf"  "ra2l1_voice_vc_ds22x.srec"
ra2l1_voice_vc_ds22x.siz: ra2l1_voice_vc_ds22x.elf
	arm-none-eabi-size --format=berkeley "ra2l1_voice_vc_ds22x.elf"
# Other Targets
clean:
	-$(file > clean_file.tmp,  $(JMP_DEPS)  $(FSY_DEPS)  $(C_UPPER_DEPS)  $(SECONDARY_SIZE)  $(SRC_UPPER_DEPS)  $(P_UPPER_DEPS)  $(S_DEPS)  $(PP_UPPER_DEPS)  $(P_DEPS)  $(FSY_UPPER_DEPS)  $(C_DEPS)  $(SRC_DEPS)  $(JMP_UPPER_DEPS)  $(PP_DEPS)  $(SX_DEPS)  $(ASM_UPPER_DEPS)  $(SX_UPPER_DEPS)  $(OBJS)  $(SECONDARY_FLASH)  $(ASM_DEPS)  $(SREC)  $(S_UPPER_DEPS)  $(MAP) )
	-@ xargs -P 2 -s 32000 -t -a clean_file.tmp $(RM)
	-@$(RM) clean_file.tmp
	-$(RM)  ra2l1_voice_vc_ds22x.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
