################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../application_functions.c \
../buzzer.c \
../control_ecu.c \
../dc_motor.c \
../external_eeprom.c \
../i2c.c \
../led.c \
../timer.c \
../uart.c 

OBJS += \
./application_functions.o \
./buzzer.o \
./control_ecu.o \
./dc_motor.o \
./external_eeprom.o \
./i2c.o \
./led.o \
./timer.o \
./uart.o 

C_DEPS += \
./application_functions.d \
./buzzer.d \
./control_ecu.d \
./dc_motor.d \
./external_eeprom.d \
./i2c.d \
./led.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


