APK=$1
echo 'sign '$APK
KEYSTORE=/Users/dyno/Documents/develop/android/android_dyno_keystore.jks
ALIAS=anddroid_dyno
ZIPALIGN=/Users/dyno/Library/Android/sdk/build-tools/24.0.0/zipalign

jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore $KEYSTORE -storepass fatpan@89 -keypass freelin@89 $APK $ALIAS
$ZIPALIGN -v 4 $APK your_project_name.apk