################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/User/BaseUserController.cpp \
../src/User/MUser.cpp \
../src/User/MUserDao.cpp \
../src/User/ProfileForm.cpp \
../src/User/RFriendsDescriptor.cpp \
../src/User/SearchUserPanel.cpp \
../src/User/UiUserListItem.cpp \
../src/User/UiUsersPanel.cpp \
../src/User/UserDescriptor.cpp \
../src/User/UserRestResponse.cpp \
../src/User/UsersComparer.cpp 

OBJS += \
./src/User/BaseUserController.o \
./src/User/MUser.o \
./src/User/MUserDao.o \
./src/User/ProfileForm.o \
./src/User/RFriendsDescriptor.o \
./src/User/SearchUserPanel.o \
./src/User/UiUserListItem.o \
./src/User/UiUsersPanel.o \
./src/User/UserDescriptor.o \
./src/User/UserRestResponse.o \
./src/User/UsersComparer.o 

CPP_DEPS += \
./src/User/BaseUserController.d \
./src/User/MUser.d \
./src/User/MUserDao.d \
./src/User/ProfileForm.d \
./src/User/RFriendsDescriptor.d \
./src/User/SearchUserPanel.d \
./src/User/UiUserListItem.d \
./src/User/UiUsersPanel.d \
./src/User/UserDescriptor.d \
./src/User/UserRestResponse.d \
./src/User/UsersComparer.d 


# Each subdirectory must supply rules for building sources it contributes
src/User/%.o: ../src/User/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -D_USE_DA_ -D_DEBUG -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/Documents/TizenWorkspace/Sparrow/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fblocks -g -pg -funwind-tables -finstrument-functions -target arm-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../arm-linux-gnueabi-gcc-4.5/ -ccc-gcc-name arm-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mtune=cortex-a8 -Wno-gnu -fPIE --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/libxml2" -I"/Users/developer/tizen-sdk/library" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include" -I"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


