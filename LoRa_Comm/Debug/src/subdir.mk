################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/data_sent.c \
../src/main.c \
../src/sensor_acq.c 

OBJS += \
./src/data_sent.o \
./src/main.o \
./src/sensor_acq.o 

C_DEPS += \
./src/data_sent.d \
./src/main.d \
./src/sensor_acq.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


