################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 



# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - ����/Debug" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - ����/Core" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - ����/User" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - ����/Peripheral/inc" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - ����/Driver" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - ����/Driver/mpu6050" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
