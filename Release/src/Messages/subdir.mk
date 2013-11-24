################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Messages/MMessage.cpp \
../src/Messages/MMessageDao.cpp \
../src/Messages/MMessageDescriptor.cpp \
../src/Messages/MessageComparer.cpp \
../src/Messages/PostMan.cpp \
../src/Messages/PostMessageOperation.cpp \
../src/Messages/RMessageSendDescriptor.cpp \
../src/Messages/RMessageSendResponse.cpp \
../src/Messages/RMessagesResponse.cpp \
../src/Messages/UiMessengerPanel.cpp 

OBJS += \
./src/Messages/MMessage.o \
./src/Messages/MMessageDao.o \
./src/Messages/MMessageDescriptor.o \
./src/Messages/MessageComparer.o \
./src/Messages/PostMan.o \
./src/Messages/PostMessageOperation.o \
./src/Messages/RMessageSendDescriptor.o \
./src/Messages/RMessageSendResponse.o \
./src/Messages/RMessagesResponse.o \
./src/Messages/UiMessengerPanel.o 

CPP_DEPS += \
./src/Messages/MMessage.d \
./src/Messages/MMessageDao.d \
./src/Messages/MMessageDescriptor.d \
./src/Messages/MessageComparer.d \
./src/Messages/PostMan.d \
./src/Messages/PostMessageOperation.d \
./src/Messages/RMessageSendDescriptor.d \
./src/Messages/RMessageSendResponse.d \
./src/Messages/RMessagesResponse.d \
./src/Messages/UiMessengerPanel.d 


# Each subdirectory must supply rules for building sources it contributes
src/Messages/%.o: ../src/Messages/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O3 -g -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


