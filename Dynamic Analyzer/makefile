################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Utils/subdir.mk
-include src/User/subdir.mk
-include src/Rest/subdir.mk
-include src/Model/subdir.mk
-include src/Messages/subdir.mk
-include src/LongPoll/subdir.mk
-include src/ImageCache/subdir.mk
-include src/Dialogs/subdir.mk
-include src/Chat/subdir.mk
-include src/Auth/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LIBRARIES += \
USER_OBJS \


# All Target
all: Sparrow.exe secondary-outputs

# Tool invocations
Sparrow.exe: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: C++ Linker'
	clang++ -o"Sparrow.exe" $(OBJS) $(USER_OBJS) $(LIBS) -L"/Users/developer/Documents/TizenWorkspace/Sparrow/lib" -L"/Users/developer/tizen-sdk/library/da/arm" -pg -target arm-tizen-linux-gnueabi -gcc-toolchain /Users/developer/tizen-sdk/tools/smart-build-interface/../arm-linux-gnueabi-gcc-4.5/ -ccc-gcc-name arm-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mtune=cortex-a8 -Xlinker --as-needed -pie -lpthread -Xlinker -rpath="/opt/usr/apps/zO13JPykpu/lib" -Xlinker -rpath="/home/developer/sdk_tools/lib" --sysroot="/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native" -L"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/lib" -L"/Users/developer/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/lib/osp" -losp-uifw -losp-appfw -losp-image -losp-json -losp-ime -losp-net -lpthread -losp-content -losp-locations -losp-telephony -losp-uix -losp-media -losp-messaging -losp-web -losp-social -losp-wifi -losp-bluetooth -losp-nfc -losp-face -losp-secure-element -losp-speech-tts -losp-shell -losp-shell-core -losp-speech-stt -losp-vision -lxml2 -lalut -lopenal
	@echo 'Finished building target: $@'
	@echo ' '

USER_OBJS: Sparrow.exe $(OBJS) $(USER_OBJS)

# Other Targets
clean:
	-$(RM) $(MO_FILES)$(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(EDJ_FILES)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Sparrow.exe
	-@echo ' '

secondary-outputs: $(MO_FILES) $(EDJ_FILES)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
