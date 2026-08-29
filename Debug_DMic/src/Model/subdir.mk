################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/Model/CybModel.asm \
../src/Model/SpeexData.asm 

OBJS += \
./src/Model/CybModel.o \
./src/Model/SpeexData.o 

ASM_DEPS += \
./src/Model/CybModel.d \
./src/Model/SpeexData.d 

SREC += \
ra2l1_voice_vc_ds22x.srec 

MAP += \
ra2l1_voice_vc_ds22x.map 


# Each subdirectory must supply rules for building sources it contributes
src/Model/%.o: ../src/Model/%.asm
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -x assembler-with-cpp -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/src" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/fsp/inc" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/fsp/inc/api" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/fsp/inc/instances" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra_gen" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra_cfg/fsp_cfg/bsp" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra_cfg/fsp_cfg" -I"." -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/arm/CMSIS_6/CMSIS/Core/Include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

