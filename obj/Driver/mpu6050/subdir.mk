################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/mpu6050/Soft_I2C.c \
../Driver/mpu6050/inv_mpu.c \
../Driver/mpu6050/inv_mpu_dmp_motion_driver.c \
../Driver/mpu6050/mpu6050.c \
../Driver/mpu6050/mpu6050_DMP.c 

C_DEPS += \
./Driver/mpu6050/Soft_I2C.d \
./Driver/mpu6050/inv_mpu.d \
./Driver/mpu6050/inv_mpu_dmp_motion_driver.d \
./Driver/mpu6050/mpu6050.d \
./Driver/mpu6050/mpu6050_DMP.d 

OBJS += \
./Driver/mpu6050/Soft_I2C.o \
./Driver/mpu6050/inv_mpu.o \
./Driver/mpu6050/inv_mpu_dmp_motion_driver.o \
./Driver/mpu6050/mpu6050.o \
./Driver/mpu6050/mpu6050_DMP.o 



# Each subdirectory must supply rules for building sources it contributes
Driver/mpu6050/%.o: ../Driver/mpu6050/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Debug" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Core" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/User" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Peripheral/inc" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Driver" -I"e:/0myhxy/000MCU/001Project/006_NeuroMemento/neuro/CH32V307VCT6 - 副本/Driver/mpu6050" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
