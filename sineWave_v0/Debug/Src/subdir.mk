################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Audio_Drivers.c \
../Src/cs43l22.c \
../Src/main.c \
../Src/sineWave.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c 

OBJS += \
./Src/Audio_Drivers.o \
./Src/cs43l22.o \
./Src/main.o \
./Src/sineWave.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/Audio_Drivers.d \
./Src/cs43l22.d \
./Src/main.d \
./Src/sineWave.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I/Users/luke/STM32CubeIDE/workspace_1.8.0/SineWaveTest3/Audio_Drivers -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/Audio_Drivers.d ./Src/Audio_Drivers.o ./Src/cs43l22.d ./Src/cs43l22.o ./Src/main.d ./Src/main.o ./Src/sineWave.d ./Src/sineWave.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32f4xx.d ./Src/system_stm32f4xx.o

.PHONY: clean-Src

