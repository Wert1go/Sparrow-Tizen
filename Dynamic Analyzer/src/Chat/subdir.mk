################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Chat/ImageAttachmentOperation.cpp \
../src/Chat/RImageSaveDescriptor.cpp \
../src/Chat/RImageUploadDataDescriptor.cpp \
../src/Chat/RImageUploadServerDescriptor.cpp \
../src/Chat/RImageUploadServerResponse.cpp \
../src/Chat/UiAttachmentListPopup.cpp \
../src/Chat/UiAttachmentView.cpp \
../src/Chat/UiChapPanel.cpp \
../src/Chat/UiChatCustomItem.cpp \
../src/Chat/UiChatForm.cpp \
../src/Chat/UiChatListItem.cpp \
../src/Chat/UiImageViewer.cpp 

OBJS += \
./src/Chat/ImageAttachmentOperation.o \
./src/Chat/RImageSaveDescriptor.o \
./src/Chat/RImageUploadDataDescriptor.o \
./src/Chat/RImageUploadServerDescriptor.o \
./src/Chat/RImageUploadServerResponse.o \
./src/Chat/UiAttachmentListPopup.o \
./src/Chat/UiAttachmentView.o \
./src/Chat/UiChapPanel.o \
./src/Chat/UiChatCustomItem.o \
./src/Chat/UiChatForm.o \
./src/Chat/UiChatListItem.o \
./src/Chat/UiImageViewer.o 

CPP_DEPS += \
./src/Chat/ImageAttachmentOperation.d \
./src/Chat/RImageSaveDescriptor.d \
./src/Chat/RImageUploadDataDescriptor.d \
./src/Chat/RImageUploadServerDescriptor.d \
./src/Chat/RImageUploadServerResponse.d \
./src/Chat/UiAttachmentListPopup.d \
./src/Chat/UiAttachmentView.d \
./src/Chat/UiChapPanel.d \
./src/Chat/UiChatCustomItem.d \
./src/Chat/UiChatForm.d \
./src/Chat/UiChatListItem.d \
./src/Chat/UiImageViewer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Chat/%.o: ../src/Chat/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -D_USE_DA_ -D_DEBUG -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fblocks -g -pg -funwind-tables -finstrument-functions -target arm-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../arm-linux-gnueabi-gcc-4.5/ -ccc-gcc-name arm-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mtune=cortex-a8 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


