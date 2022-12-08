# Variables
CRASHPAD_DIR="/home/bobby/Desktop/github/crashpad"
ANDROID_ABIS=( "arm64-v8a" "armeabi-v7a" "x86" "x86_64" )
PROJECT_DIR="/home/bobby/Desktop/github/my-android-crasher"

# Start in the Crashpad dir
cd $CRASHPAD_DIR

# Update Crashpad
git pull -r
gclient sync

# Copy .h Includes
rsync -avh --include='*/' --include='*.h' --exclude='*' --prune-empty-dirs ./ $PROJECT_DIR/app/src/main/cpp/crashpad/include

for ANDROID_ABI in "${ANDROID_ABIS[@]}"
do
    CRASHPAD_GN="out/$ANDROID_ABI"
    CRASHPAD_OUT="$CRASHPAD_DIR/$CRASHPAD_GN"

    # Build Crashpad
    ninja -C $CRASHPAD_OUT

    # Copy chrome_buildflags.h
    mkdir -p $PROJECT_DIR/app/src/main/cpp/crashpad/include/build
    cp $CRASHPAD_OUT/gen/build/chromeos_buildflags.h $PROJECT_DIR/app/src/main/cpp/crashpad/include/build

    # Copy Libraries
    mkdir -p $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
    mkdir -p $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI/base
    mkdir -p $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI/client
    mkdir -p $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI/util
    cp $CRASHPAD_OUT/obj/client/libclient.a $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI/client
    cp $CRASHPAD_OUT/obj/client/libcommon.a $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI/client
    cp $CRASHPAD_OUT/obj/util/libutil.a $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI/util
    cp $CRASHPAD_OUT/obj/third_party/mini_chromium/mini_chromium/base/libbase.a $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI/base

    # Copy Handler
    cp $CRASHPAD_OUT/libcrashpad_handler.so $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
    cp $CRASHPAD_OUT/libcrashpad_handler_trampoline.so $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI

done