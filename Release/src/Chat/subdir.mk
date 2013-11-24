################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Chat/UiChapPanel.cpp \
../src/Chat/UiChatCustomItem.cpp \
../src/Chat/UiChatForm.cpp \
../src/Chat/UiChatListItem.cpp 

OBJS += \
./src/Chat/UiChapPanel.o \
./src/Chat/UiChatCustomItem.o \
./src/Chat/UiChatForm.o \
./src/Chat/UiChatListItem.o 

CPP_DEPS += \
./src/Chat/UiChapPanel.d \
./src/Chat/UiChatCustomItem.d \
./src/Chat/UiChatForm.d \
./src/Chat/UiChatListItem.d 


# Each subdirectory must supply rules for building sources it contributes
src/Chat/%.o: ../src/Chat/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O3 -g -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


