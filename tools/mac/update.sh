# Variables
CRASHPAD_DIR="/Users/bobby/Desktop/bugsplat/crashpad"
ANDROID_ABI="arm64-v8a"
CRASHPAD_GN="out/$ANDROID_ABI"
CRASHPAD_OUT="$CRASHPAD_DIR/$CRASHPAD_GN"
PROJECT_DIR="/Users/bobby/Desktop/bugsplat/my-android-crasher"

# Start in the Crashpad dir
cd $CRASHPAD_DIR

# Update Crashpad
git pull -r
gclient sync

# Build Crashpad
ninja -C $CRASHPAD_OUT

# Copy .h Includess
rsync -avh --include='*/' --include='*.h' --exclude='*' --prune-empty-dirs ./ $PROJECT_DIR/app/src/main/cpp/crashpad/include

# Copy Libraries
cp -R $CRASHPAD_OUT/obj/client $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
cp -R $CRASHPAD_OUT/obj/util $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
cp -R $CRASHPAD_OUT/obj/third_party/mini_chromium/mini_chromium/base $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI

# Copy Handler
cp $CRASHPAD_OUT/libcrashpad_handler.so $PROJECT_DIR/app/src/main/cpp/crashpad/lib/$ANDROID_ABI
