#!/bin/sh
# Builds the port's Android app, an APK, in its container (docs/PORT.md, Android): tools/da sh port/android/build.sh
#   1. the game as libmain.so for each kind of processor (arm64-v8a for phones, x86_64 for the emulator) with the NDK, beside the
#      image's libSDL3.so for it;
#   2. SDL's Java side (from SDL's source) and the port's activity, compiled against the platform and made into dex;
#   3. the manifest and icon, then all of it zipped, aligned and signed: with $ANDROID_KEYSTORE (its password in the file
#      $ANDROID_KEYSTORE_PASS_FILE, or in $ANDROID_KEYSTORE_PASS) if that's set, as tools/da sets them from CRASH_BASH_ANDROID_KEY for
#      a release, or else with a debug key kept in build/port/android/, which does to install it for testing.
# It makes build/port/android/crashbash.apk, its version the port's own (port/CMakeLists.txt).
set -e
cd "$(dirname "$0")/../.."
OUT=build/port/android
VERSION=$(sed -n 's/^project(crash_bash_pc VERSION \([0-9.]*\) .*/\1/p' port/CMakeLists.txt)
CODE=$(echo "$VERSION" | awk -F. '{ print $1 * 10000 + $2 * 100 + $3 }')
TARGET_SDK=$(basename "$(dirname "$ANDROID_PLATFORM_JAR")" | sed 's/^android-//')

rm -rf $OUT/apk $OUT/classes
mkdir -p $OUT/apk $OUT/classes

for abi in arm64-v8a x86_64; do
    cmake -S port -B $OUT/$abi -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" -DBASH_RELEASE=ON \
        -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" -DANDROID_ABI=$abi -DANDROID_PLATFORM=$ANDROID_MIN_SDK \
        -DSDL3_DIR=/opt/sdl3-android/$abi/lib/cmake/SDL3 > $OUT/configure-$abi.log
    cmake --build $OUT/$abi
    mkdir -p $OUT/apk/lib/$abi
    cp $OUT/$abi/libmain.so /opt/sdl3-android/$abi/lib/libSDL3.so $OUT/apk/lib/$abi/
done

javac -nowarn -encoding UTF-8 -source 8 -target 8 -Xlint:-options -bootclasspath "$ANDROID_PLATFORM_JAR" -d $OUT/classes \
    $(find "$SDL3_SOURCE/android-project/app/src/main/java" port/android/java -name '*.java')
"$ANDROID_BUILD_TOOLS/d8" --release --min-api $ANDROID_MIN_SDK --lib "$ANDROID_PLATFORM_JAR" --output $OUT/apk $(find $OUT/classes -name '*.class')

"$ANDROID_BUILD_TOOLS/aapt2" compile --dir port/android/res -o $OUT/res.zip
"$ANDROID_BUILD_TOOLS/aapt2" link -o $OUT/unsigned.apk -I "$ANDROID_PLATFORM_JAR" --manifest port/android/AndroidManifest.xml \
    --min-sdk-version $ANDROID_MIN_SDK --target-sdk-version $TARGET_SDK --version-code $CODE --version-name $VERSION $OUT/res.zip
(cd $OUT/apk && zip -qr ../unsigned.apk classes.dex lib)
"$ANDROID_BUILD_TOOLS/zipalign" -f -P 16 4 $OUT/unsigned.apk $OUT/aligned.apk

if [ -n "$ANDROID_KEYSTORE" ]; then
    KEYSTORE="$ANDROID_KEYSTORE"
    if [ -n "$ANDROID_KEYSTORE_PASS_FILE" ]; then
        PASS="file:$ANDROID_KEYSTORE_PASS_FILE"
    else
        PASS="pass:$ANDROID_KEYSTORE_PASS"
    fi
    SIGNED_WITH="the release key"
else
    KEYSTORE=$OUT/debug.keystore
    PASS=pass:android
    SIGNED_WITH="a debug key, for testing"
    [ -f $KEYSTORE ] || keytool -genkeypair -keystore $KEYSTORE -storepass android -keypass android -alias androiddebugkey \
        -dname "CN=Android Debug,O=Android,C=US" -keyalg RSA -keysize 2048 -validity 10000 > /dev/null 2>&1
fi
"$ANDROID_BUILD_TOOLS/apksigner" sign --ks "$KEYSTORE" --ks-pass "$PASS" --out $OUT/crashbash.apk $OUT/aligned.apk
rm -f $OUT/unsigned.apk $OUT/aligned.apk $OUT/crashbash.apk.idsig
echo "$OUT/crashbash.apk: version $VERSION ($CODE), $(stat -c %s $OUT/crashbash.apk) bytes, signed with $SIGNED_WITH"
