################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/archive_manager.c \
../source/event_threads.c \
../source/font_b.c \
../source/gfx.c \
../source/language.c \
../source/libfont.c \
../source/main.c \
../source/manager.c \
../source/osk_input.c \
../source/pad.c \
../source/sysregistry.c \
../source/ttf_render.c \
../source/utils.c 

OBJS += \
./source/archive_manager.o \
./source/event_threads.o \
./source/font_b.o \
./source/gfx.o \
./source/language.o \
./source/libfont.o \
./source/main.o \
./source/manager.o \
./source/osk_input.o \
./source/pad.o \
./source/sysregistry.o \
./source/ttf_render.o \
./source/utils.o 

C_DEPS += \
./source/archive_manager.d \
./source/event_threads.d \
./source/font_b.d \
./source/gfx.d \
./source/language.d \
./source/libfont.d \
./source/main.d \
./source/manager.d \
./source/osk_input.d \
./source/pad.d \
./source/sysregistry.d \
./source/ttf_render.d \
./source/utils.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D"PS3DEV=/usr/local/ps3dev" -D"PSL1GHT=/usr/local/ps3dev/psl1ght" -D"PS3=/usr/local/ps3dev/ps3" -D"PORTLIBS}=/usr/local/ps3dev/portlibs" -I/usr/local/ps3dev/portlibs/ppu/include -I/usr/local/ps3dev/ppu/include -I/usr/local/ps3dev/spu/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


