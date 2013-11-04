################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AuthForm.cpp \
../src/AuthManager.cpp \
../src/AuthWebForm.cpp \
../src/FormFactory.cpp \
../src/MainForm.cpp \
../src/SceneRegister.cpp \
../src/SettingsForm.cpp \
../src/Sparrow.cpp \
../src/SparrowEntry.cpp \
../src/SparrowFrame.cpp 

OBJS += \
./src/AuthForm.o \
./src/AuthManager.o \
./src/AuthWebForm.o \
./src/FormFactory.o \
./src/MainForm.o \
./src/SceneRegister.o \
./src/SettingsForm.o \
./src/Sparrow.o \
./src/SparrowEntry.o \
./src/SparrowFrame.o 

CPP_DEPS += \
./src/AuthForm.d \
./src/AuthManager.d \
./src/AuthWebForm.d \
./src/FormFactory.d \
./src/MainForm.d \
./src/SceneRegister.d \
./src/SettingsForm.d \
./src/Sparrow.d \
./src/SparrowEntry.d \
./src/SparrowFrame.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -D_DEBUG -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O0 -g3 -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


