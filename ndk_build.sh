
#!/bin/sh

if [ "$1" = "-r" ];then
echo "1. Release"
export C3_DEBUG_BUILD=0
else
echo "1. Debug"
export C3_DEBUG_BUILD=1
fi

export C3_CLEAN_BUILD
if [ "$1" = "-b" -o "$2" = "-b" ];then
echo "----clean and build"
export C3_CLEAN_BUILD=-B
fi

export DEMO_BUILD_SETTING=/cygdrive/e/works/Project/AppPlatform/phonesdk_svn_new_0718/Android/C++Demo/C3BuildSetting.mk

ndk-build NDK_DEBUG=$C3_DEBUG_BUILD $C3_CLEAN_BUILD -C android

