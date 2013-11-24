################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/User/BaseUserController.cpp \
../src/User/MUser.cpp \
../src/User/MUserDao.cpp \
../src/User/RFriendsDescriptor.cpp \
../src/User/UiUserListItem.cpp \
../src/User/UiUsersPanel.cpp \
../src/User/UserDescriptor.cpp \
../src/User/UserRestResponse.cpp \
../src/User/UsersComparer.cpp 

OBJS += \
./src/User/BaseUserController.o \
./src/User/MUser.o \
./src/User/MUserDao.o \
./src/User/RFriendsDescriptor.o \
./src/User/UiUserListItem.o \
./src/User/UiUsersPanel.o \
./src/User/UserDescriptor.o \
./src/User/UserRestResponse.o \
./src/User/UsersComparer.o 

CPP_DEPS += \
./src/User/BaseUserController.d \
./src/User/MUser.d \
./src/User/MUserDao.d \
./src/User/RFriendsDescriptor.d \
./src/User/UiUserListItem.d \
./src/User/UiUsersPanel.d \
./src/User/UserDescriptor.d \
./src/User/UserRestResponse.d \
./src/User/UsersComparer.d 


# Each subdirectory must supply rules for building sources it contributes
src/User/%.o: ../src/User/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O3 -g -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/ -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


