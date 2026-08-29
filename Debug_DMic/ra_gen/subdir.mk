################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/elc_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/elc_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/elc_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

SREC += \
ra2l1_voice_vc_ds22x.srec 

MAP += \
ra2l1_voice_vc_ds22x.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -DMCU_BOARD=MCU_BOARD_RA2L1_VOICE -DAUDIO_RECORD=AUDIO_RECORD_SPI -DUART_LOG -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/src" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/fsp/inc" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/fsp/inc/api" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/fsp/inc/instances" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra_gen" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra_cfg/fsp_cfg/bsp" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra_cfg/fsp_cfg" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/src/DSpotter" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/src/DSpotter/include" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/src/Util" -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/src/Speex/include" -I"." -I"C:/Users/a5166677/e2_studio/workspace/ra2l1_voice_vc_ds22x/ra2l1_voice_vc_ds22x/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

