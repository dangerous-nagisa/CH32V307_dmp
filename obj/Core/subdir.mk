################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/core_riscv.c 

C_DEPS += \
./Core/core_riscv.d 

OBJS += \
./Core/core_riscv.o 



# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Debug" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Core" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/User" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Peripheral/inc" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Driver" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Driver/mpu6050" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
