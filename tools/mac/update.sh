# Variables
CRASHPAD_DIR="/Users/bobby/Desktop/bugsplat/crashpad"
ANDROID_ABIS=( "arm64-v8a" "armeabi-v71" "x86" "x86_64" )
PROJECT_DIR="/Users/bobby/Desktop/bugsplat/my-android-crasher"

# Start in the Crashpad dir
cd $CRASHPAD_DIR

# Update Crashpad
#git pull -r
#gclient sync

for ANDROID_ABI in "${ANDROID_ABIS[@]}"
do
    CRASHPAD_GN="out/$ANDROID_ABI"
    CRASHPAD_OUT="$CRASHPAD_DIR/$CRASHPAD_GN"

    # Build Crashpad
    ninja -C $CRASHPAD_OUT

    # Copy .h Includess
    rsync -avh --include='*/' --include='*.h' --exclude='*' --prune-empty-dirs ./ $PROJECT_DIR/app/src/main/cpp/crashpad/include
    mkdir -p $PROJECT_DIR/app/src/main/cpp/crashpad/include/build
    cp $CRASHPAD_OUT/gen/build/chromeos_buildflags.h $PROJECT_DIR/app/src/main/cpp/crashpad/include/build

    # Copy Libraries
    cp -R $CRASHPAD_OUT/obj/client $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
    cp -R $CRASHPAD_OUT/obj/util $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
    cp -R $CRASHPAD_OUT/obj/third_party/mini_chromium/mini_chromium/base $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI

    # Copy Handler
    cp $CRASHPAD_OUT/libcrashpad_handler.so $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
    cp $CRASHPAD_OUT/libcrashpad_handler_trampoline.so $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI

done