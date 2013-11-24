################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LongPoll/LongPollConnection.cpp \
../src/LongPoll/LongPollDescriptor.cpp \
../src/LongPoll/LongPollObject.cpp \
../src/LongPoll/LongPollResponse.cpp \
../src/LongPoll/LongPollServerDataDescriptor.cpp \
../src/LongPoll/LongPollServerDataResponse.cpp 

OBJS += \
./src/LongPoll/LongPollConnection.o \
./src/LongPoll/LongPollDescriptor.o \
./src/LongPoll/LongPollObject.o \
./src/LongPoll/LongPollResponse.o \
./src/LongPoll/LongPollServerDataDescriptor.o \
./src/LongPoll/LongPollServerDataResponse.o 

CPP_DEPS += \
./src/LongPoll/LongPollConnection.d \
./src/LongPoll/LongPollDescriptor.d \
./src/LongPoll/LongPollObject.d \
./src/LongPoll/LongPollResponse.d \
./src/LongPoll/LongPollServerDataDescriptor.d \
./src/LongPoll/LongPollServerDataResponse.d 


# Each subdirectory must supply rules for building sources it contributes
src/LongPoll/%.o: ../src/LongPoll/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O3 -g -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


