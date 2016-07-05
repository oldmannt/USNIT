# Android makefile for helloworld shared lib, jni wrapper around libhelloworld C API
 
APP_ABI := armeabi-v7a
APP_OPTIM := release
APP_PLATFORM := android-19
# GCC 4.9 Toolchain
NDK_TOOLCHAIN_VERSION = 4.9
# GNU libc++ is the only Android STL which supports C++11 features
APP_STL := gnustl_shared
#APP_BUILD_SCRIPT := ./Android.mk
APP_MODULES := gearsbox_jni