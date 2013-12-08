################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dialogs/CreateChatForm.cpp \
../src/Dialogs/CreateConversationForm.cpp \
../src/Dialogs/MDialog.cpp \
../src/Dialogs/MDialogDao.cpp \
../src/Dialogs/MDialogsDescriptor.cpp \
../src/Dialogs/RDialogResponse.cpp \
../src/Dialogs/SelectChatUsersForm.cpp \
../src/Dialogs/UiCustomItemBackground.cpp \
../src/Dialogs/UiDialogCustomItem.cpp \
../src/Dialogs/UiDialogListItem.cpp \
../src/Dialogs/UiMessagesPanel.cpp 

OBJS += \
./src/Dialogs/CreateChatForm.o \
./src/Dialogs/CreateConversationForm.o \
./src/Dialogs/MDialog.o \
./src/Dialogs/MDialogDao.o \
./src/Dialogs/MDialogsDescriptor.o \
./src/Dialogs/RDialogResponse.o \
./src/Dialogs/SelectChatUsersForm.o \
./src/Dialogs/UiCustomItemBackground.o \
./src/Dialogs/UiDialogCustomItem.o \
./src/Dialogs/UiDialogListItem.o \
./src/Dialogs/UiMessagesPanel.o 

CPP_DEPS += \
./src/Dialogs/CreateChatForm.d \
./src/Dialogs/CreateConversationForm.d \
./src/Dialogs/MDialog.d \
./src/Dialogs/MDialogDao.d \
./src/Dialogs/MDialogsDescriptor.d \
./src/Dialogs/RDialogResponse.d \
./src/Dialogs/SelectChatUsersForm.d \
./src/Dialogs/UiCustomItemBackground.d \
./src/Dialogs/UiDialogCustomItem.d \
./src/Dialogs/UiDialogListItem.d \
./src/Dialogs/UiMessagesPanel.d 


# Each subdirectory must supply rules for building sources it contributes
src/Dialogs/%.o: ../src/Dialogs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -D_USE_DA_ -D_DEBUG -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fblocks -g -pg -funwind-tables -finstrument-functions -target arm-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../arm-linux-gnueabi-gcc-4.5/ -ccc-gcc-name arm-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mtune=cortex-a8 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


