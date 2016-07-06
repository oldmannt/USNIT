# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE := usnit_jni
LOCAL_MODULE_SUFFIX := .a
LOCAL_MODULE_TARGET_ARCH := $(TARGET_$(GYP_VAR_PREFIX)ARCH)
LOCAL_SDK_VERSION := 19
gyp_intermediate_dir := $(call local-intermediates-dir,,$(GYP_VAR_PREFIX))
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared,,,$(GYP_VAR_PREFIX))

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES :=

GYP_GENERATED_OUTPUTS :=

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

LOCAL_GENERATED_SOURCES :=

GYP_COPIED_SOURCE_ORIGIN_DIRS :=

LOCAL_SRC_FILES := \
	generated-src/jni/NativeUilogicGen.cpp \
	generated-src/jni/NativeUsnitEventGen.cpp


# Flags passed to both C and C++ files.
MY_CFLAGS_Debug := \
	-gdwarf-2 \
	-Werror \
	-Wall \
	-Wextra \
	-Wno-missing-field-initializers \
	-g \
	-O0 \
	-DDEBUG=1

MY_DEFS_Debug := \
	'-DDEBUG=1'


# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES_Debug := \
	$(LOCAL_PATH)/generated-src/jni \
	$(LOCAL_PATH)/generated-src/cpp \
	$(LOCAL_PATH)/cpp \
	$(LOCAL_PATH)/gearsbox/deps/djinni/support-lib/jni \
	$(LOCAL_PATH)/gearsbox/generated-src/cpp \
	$(LOCAL_PATH)/gearsbox/interface \
	$(LOCAL_PATH)/gearsbox/src/cpp/ui \
	$(LOCAL_PATH)/gearsbox/deps/sqlite3 \
	$(LOCAL_PATH)/gearsbox/deps/curl \
	$(LOCAL_PATH)/gearsbox/deps/curl/curl


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Debug := \
	-std=c++11 \
	-frtti \
	-fexceptions \
	-Wno-literal-suffix


# Flags passed to both C and C++ files.
MY_CFLAGS_Release := \
	-gdwarf-2 \
	-Werror \
	-Wall \
	-Wextra \
	-Wno-missing-field-initializers \
	-Os \
	-fomit-frame-pointer \
	-fdata-sections \
	-ffunction-sections \
	-DNDEBUG=1

MY_DEFS_Release := \
	'-DNDEBUG=1'


# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES_Release := \
	$(LOCAL_PATH)/generated-src/jni \
	$(LOCAL_PATH)/generated-src/cpp \
	$(LOCAL_PATH)/cpp \
	$(LOCAL_PATH)/gearsbox/deps/djinni/support-lib/jni \
	$(LOCAL_PATH)/gearsbox/generated-src/cpp \
	$(LOCAL_PATH)/gearsbox/interface \
	$(LOCAL_PATH)/gearsbox/src/cpp/ui \
	$(LOCAL_PATH)/gearsbox/deps/sqlite3 \
	$(LOCAL_PATH)/gearsbox/deps/curl \
	$(LOCAL_PATH)/gearsbox/deps/curl/curl


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Release := \
	-std=c++11 \
	-frtti \
	-fexceptions \
	-Wno-literal-suffix


LOCAL_CFLAGS := $(MY_CFLAGS_$(GYP_CONFIGURATION)) $(MY_DEFS_$(GYP_CONFIGURATION))
LOCAL_C_INCLUDES := $(GYP_COPIED_SOURCE_ORIGIN_DIRS) $(LOCAL_C_INCLUDES_$(GYP_CONFIGURATION))
LOCAL_CPPFLAGS := $(LOCAL_CPPFLAGS_$(GYP_CONFIGURATION))
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)
### Rules for final target.
include $(BUILD_STATIC_LIBRARY)
