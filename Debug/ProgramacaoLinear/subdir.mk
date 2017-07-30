################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ProgramacaoLinear/ModeloPL.cpp \
../ProgramacaoLinear/ModeloPLRacional.cpp 

OBJS += \
./ProgramacaoLinear/ModeloPL.o \
./ProgramacaoLinear/ModeloPLRacional.o 

CPP_DEPS += \
./ProgramacaoLinear/ModeloPL.d \
./ProgramacaoLinear/ModeloPLRacional.d 


# Each subdirectory must supply rules for building sources it contributes
ProgramacaoLinear/%.o: ../ProgramacaoLinear/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


