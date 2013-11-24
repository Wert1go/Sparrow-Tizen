################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dialogs/CreateChatForm.cpp \
../src/Dialogs/MDialog.cpp \
../src/Dialogs/MDialogDao.cpp \
../src/Dialogs/MDialogsDescriptor.cpp \
../src/Dialogs/RDialogResponse.cpp \
../src/Dialogs/UiCustomItemBackground.cpp \
../src/Dialogs/UiDialogCustomItem.cpp \
../src/Dialogs/UiDialogListItem.cpp \
../src/Dialogs/UiMessagesPanel.cpp 

OBJS += \
./src/Dialogs/CreateChatForm.o \
./src/Dialogs/MDialog.o \
./src/Dialogs/MDialogDao.o \
./src/Dialogs/MDialogsDescriptor.o \
./src/Dialogs/RDialogResponse.o \
./src/Dialogs/UiCustomItemBackground.o \
./src/Dialogs/UiDialogCustomItem.o \
./src/Dialogs/UiDialogListItem.o \
./src/Dialogs/UiMessagesPanel.o 

CPP_DEPS += \
./src/Dialogs/CreateChatForm.d \
./src/Dialogs/MDialog.d \
./src/Dialogs/MDialogDao.d \
./src/Dialogs/MDialogsDescriptor.d \
./src/Dialogs/RDialogResponse.d \
./src/Dialogs/UiCustomItemBackground.d \
./src/Dialogs/UiDialogCustomItem.d \
./src/Dialogs/UiDialogListItem.d \
./src/Dialogs/UiMessagesPanel.d 


# Each subdirectory must supply rules for building sources it contributes
src/Dialogs/%.o: ../src/Dialogs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O3 -g -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


