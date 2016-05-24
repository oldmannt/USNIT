ios_proj: usnit.gyp ./gearsbox/deps/djinni/support-lib/support_lib.gyp usnit.djinni
	-rm -rf generated-src
	cd gearsbox && make ios_proj
	sh ./run_djinni.sh
	./gearsbox/deps/gyp/gyp --depth=. -f xcode -DOS=ios --generator-output ./ios/build_ios -Igearsbox/deps/djinni/common.gypi ./usnit.gyp

ios: ios_proj
	xcodebuild -workspace ios/usnit.xcworkspace \
		-scheme gearsbox \
		-configuration 'Debug' \
		-sdk iphonesimulator

android_proj: usnit.gyp ./gearsbox/deps/djinni/support-lib/support_lib.gyp usnit.djinni
	sh ./run_djinni.sh
	ANDROID_BUILD_TOP=$(shell dirname `which ndk-build`) ./gearsbox/deps/gyp/gyp --depth=. -f android -DOS=android -Igearsbox/deps/djinni/common.gypi ./usnit.gyp --root-target=usnit_jni

android: android_proj
	cd android/usnit/ && ./gradlew app:assembleDebug
	@echo "Apks produced at:"
	@python ./gearsbox/deps/djinni/example/glob.py ./ '*.apk'


clean_ios:
	-rm -rf build_ios
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

