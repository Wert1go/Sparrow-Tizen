################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FormFactory.cpp \
../src/MainForm.cpp \
../src/PanelFactory.cpp \
../src/SceneRegister.cpp \
../src/SettingsForm.cpp \
../src/Sparrow.cpp \
../src/SparrowEntry.cpp \
../src/SparrowFrame.cpp \
../src/UpdateUnit.cpp 

OBJS += \
./src/FormFactory.o \
./src/MainForm.o \
./src/PanelFactory.o \
./src/SceneRegister.o \
./src/SettingsForm.o \
./src/Sparrow.o \
./src/SparrowEntry.o \
./src/SparrowFrame.o \
./src/UpdateUnit.o 

CPP_DEPS += \
./src/FormFactory.d \
./src/MainForm.d \
./src/PanelFactory.d \
./src/SceneRegister.d \
./src/SettingsForm.d \
./src/Sparrow.d \
./src/SparrowEntry.d \
./src/SparrowFrame.d \
./src/UpdateUnit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -D_USE_DA_ -D_DEBUG -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fblocks -g -pg -funwind-tables -finstrument-functions -target arm-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../arm-linux-gnueabi-gcc-4.5/ -ccc-gcc-name arm-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mtune=cortex-a8 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


