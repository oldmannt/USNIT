djinni:gearsbox/djinni/gearsbox.djinni gearsbox/djinni/ui.djinni usnit.djinni
	-rm -rf generated-src
	cd gearsbox/ && make djinni
	sh ./run_djinni.sh


ios_proj: djinni usnit.gyp ./gearsbox/deps/djinni/support-lib/support_lib.gyp 
	./gearsbox/deps/gyp/gyp --depth=. -f xcode -DOS=ios --generator-output ./ios/build_ios -Igearsbox/deps/djinni/common.gypi ./usnit.gyp --root-target=usnit_objc

ios: ios_proj
	xcodebuild -workspace ios/usnit.xcworkspace \
		-scheme gearsbox \
		-configuration 'Debug' \
		-sdk iphonesimulator

android_proj: djinni usnit.gyp ./gearsbox/deps/djinni/support-lib/support_lib.gyp usnit.djinni
	sh ./run_djinni.sh
	ANDROID_BUILD_TOP=$(shell dirname `which ndk-build`) ./gearsbox/deps/gyp/gyp --depth=. -f android -DOS=android -Igearsbox/deps/djinni/common.gypi ./usnit.gyp --root-target=usnit_jni

.PHONY: android
android: 
	cd android/usnit/ && chmod +x gradlew &&./gradlew app:assembleRelease
	cp android/usnit/app/build/outputs/apk/app-armv7-release.apk 	android/usnit-`date +%Y%m%d-%H%M`-armv7.apk
	cp android/usnit/app/build/outputs/apk/app-armv64-release.apk 	android/usnit-`date +%Y%m%d-%H%M`-arm64.apk
	cp android/usnit/app/build/outputs/apk/app-fat-release.apk 		android/usnit-`date +%Y%m%d-%H%M`-fat.apk
	cp android/usnit/app/build/outputs/apk/app-x64-release.apk	android/usnit-`date +%Y%m%d-%H%M`-x64.apk
	cp android/usnit/app/build/outputs/apk/app-arm-release.apk	android/usnit-`date +%Y%m%d-%H%M`-arm.apk
	@echo "Apks produced at:"
	@python ./gearsbox/deps/djinni/example/glob.py ./ '*.apk'

ndk_build: GypAndroid.mk android/jni/Android.mk android/jni/Application.mk
	sh ./ndk_build.sh


clean_ios:
	-rm -rf build
	-xcodebuild -workspace ios/usnit.xcworkspace \
		-scheme gearsbox \
		-configuration 'Debug' \
		-sdk iphonesimulator clean

clean_android:
	#-rm -f GypAndroid.mk
	-rm -rf android/usnit/build
	-rm -rf android/usnit/app/build

clean: clean_ios clean_android

deps:
	git submodule add https://github.com/oldmannt/gearsbox_client.git gearsbox

mac_proj: djinni usnit.gyp ./gearsbox/deps/djinni/support-lib/support_lib.gyp 
	gearsbox/deps/gyp/gyp --depth=. -f xcode -DOS=mac --generator-output ./build_mac -Igearsbox/deps/djinni/common.gypi ./usnit.gyp --root-target usnit_objc --root-target test

test: mac_proj
	xcodebuild -project build_mac/usnit.xcodeproj -configuration Debug -target test | cat && ./build/Debug/test
