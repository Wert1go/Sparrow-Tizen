################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Messages/AttachmentItem.cpp \
../src/Messages/MAttachment.cpp \
../src/Messages/MAttachmentDao.cpp \
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
./src/Messages/AttachmentItem.o \
./src/Messages/MAttachment.o \
./src/Messages/MAttachmentDao.o \
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
./src/Messages/AttachmentItem.d \
./src/Messages/MAttachment.d \
./src/Messages/MAttachmentDao.d \
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
	clang++ -I"pch" -D_USE_DA_ -D_DEBUG -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fblocks -g -pg -funwind-tables -finstrument-functions -target arm-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../arm-linux-gnueabi-gcc-4.5/ -ccc-gcc-name arm-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mtune=cortex-a8 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


