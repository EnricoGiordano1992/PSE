################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/div_RTL.cc \
../src/main_root_RTL.cc \
../src/mul_LT_testbench.cc \
../src/root_RTL_transactor.cc \
../src/support.cc 

CC_DEPS += \
./src/div_RTL.d \
./src/main_root_RTL.d \
./src/mul_LT_testbench.d \
./src/root_RTL_transactor.d \
./src/support.d 

OBJS += \
./src/div_RTL.o \
./src/main_root_RTL.o \
./src/mul_LT_testbench.o \
./src/root_RTL_transactor.o \
./src/support.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/systemc-2.3.0/include -I/Users/Ale/Documents/workspace/transactor_rtl_tlm/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


