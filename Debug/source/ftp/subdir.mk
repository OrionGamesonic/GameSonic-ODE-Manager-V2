################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ftp/client.c \
../source/ftp/ftp.c \
../source/ftp/functions.c \
../source/ftp/server.c 

OBJS += \
./source/ftp/client.o \
./source/ftp/ftp.o \
./source/ftp/functions.o \
./source/ftp/server.o 

C_DEPS += \
./source/ftp/client.d \
./source/ftp/ftp.d \
./source/ftp/functions.d \
./source/ftp/server.d 


# Each subdirectory must supply rules for building sources it contributes
source/ftp/%.o: ../source/ftp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


