[![bugsplat-github-banner-basic-outline](https://user-images.githubusercontent.com/20464226/149019306-3186103c-5315-4dad-a499-4fd1df408475.png)](https://bugsplat.com)
<br/>
# <div align="center">BugSplat</div> 
### **<div align="center">Crash and error reporting built for busy developers.</div>**
<div align="center">
    <a href="https://twitter.com/BugSplatCo">
        <img alt="Follow @bugsplatco on Twitter" src="https://img.shields.io/twitter/follow/bugsplatco?label=Follow%20BugSplat&style=social">
    </a>
    <a href="https://discord.gg/K4KjjRV5ve">
        <img alt="Join BugSplat on Discord" src="https://img.shields.io/discord/664965194799251487?label=Join%20Discord&logo=Discord&style=social">
    </a>
</div>

## 👋 Introduction

This repository contains an [Android NDK](https://developer.android.com/ndk) project integrated with [Crashpad](https://chromium.googlesource.com/crashpad/crashpad/+/master/README.md) and [BugSplat](https://www.bugsplat.com). This repository includes pre-built versions of the Crashpad libraries as well as the Breakpad tools [dump_syms](https://github.com/google/breakpad/tree/master/src/tools/linux/dump_syms) and [symupload](https://github.com/google/breakpad/tree/master/src/tools/linux/symupload) that are used to generate [.sym](https://chromium.googlesource.com/breakpad/breakpad/+/master/docs/processor_design.md#symbol-files) files so minidumps can be symbolicated by BugSplat.

NDK crash reporting is supported for x86_64, arm64-v8a, and armeabi-v7a ABIs on Android 10+.

## 🏗️ Building

To get started, install [Android Studio](https://developer.android.com/studio/) and create an [Android emulator](https://developer.android.com/studio/run/managing-avds) or configure a [device for debugging](https://developer.android.com/studio/run/device).

Next, clone this repository.

```sh
git clone https://github.com/BugSplat-Git/my-android-crasher.git
```

[Build and run](https://developer.android.com/studio/run) the app, this should cause the Android NDK to be installed if it isn't installed already.

## 🗿 Symbols 

[Symbols](https://docs.bugsplat.com/introduction/development/working-with-symbol-files#what-are-symbols) or `.sym` files are required to display function names and line numbers in Android NDK crash reports. To generate `.sym` files you'll need to run `dump_syms` on a Linux system or a Windows system via [WSL](https://learn.microsoft.com/en-us/windows/wsl/about).

You can run one the [symupload scripts](https://github.com/BugSplat-Git/my-android-crasher/tree/master/tools) in the tools folder to generate and upload symbols during development. It's important that the [version you use in the URL](https://github.com/BugSplat-Git/my-android-crasher/blob/master/tools/symupload-linux.sh#L5) when uploading symbols matches the [version used for crash uploads](https://github.com/BugSplat-Git/my-android-crasher/blob/master/app/src/main/cpp/native-lib.cpp#L42). At a minimum, symbols should be uploaded for each public release of your app.

## 💥 Testing

Once you've uploaded symbols, run the application without making any code changes so the project isn't rebuilt. If the IDE does trigger a build, be sure to re-upload symbols. When the app loads, tap the **Crash** button to generate a crash.

Navigate to BugSplat's [Crashes](https://app.bugsplat.com/v2/crashes?c0=appName&f0=CONTAINS&v0=my-android-crasher) page to see a list of recent my-android-crasher crashes. Click the link in the ID column to see full debug information for the generated crash.

If everything went correctly, you should see something that resembles the following.

![BugSplat Crash Page](https://user-images.githubusercontent.com/2646053/208204524-2c55283f-410d-4b2e-b8de-51f58ce91bac.png)

## ⚙️ Integration

If you'd like to integrate Crashpad into your own project follow these steps:

1. Copy the crashpad folder to your project
2. Add `sourceSets` and `packagingOptions` to [build.gradle](https://github.com/BugSplat-Git/my-android-crasher/blob/master/app/build.gradle).

```
sourceSets {
    main {
        jniLibs.srcDirs = ['src/main/cpp/crashpad/lib']
    }
}

packagingOptions {
    jniLibs {
        keepDebugSymbols += ['*/armeabi/*.so', '*/armeabi-v7a/*.so', '*/x86_64/*.so']
    }
}
```

3. Add [CMakeLists.txt](https://github.com/BugSplat-Git/my-android-crasher/blob/master/app/src/main/cpp/CMakeLists.txt) rules to link with `libcommon.a`, `libclient.a`, `libutil.a`, and `libbase.a`, and include the Crashpad headers.

```
# Crashpad Libraries
add_library(crashpad_client_common STATIC IMPORTED)
set_property(TARGET crashpad_client_common PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/client/libcommon.a)

add_library(crashpad_client STATIC IMPORTED)
set_property(TARGET crashpad_client PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/client/libclient.a)

add_library(crashpad_util STATIC IMPORTED)
set_property(TARGET crashpad_util PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/util/libutil.a)

add_library(base STATIC IMPORTED)
set_property(TARGET base PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/base/libbase.a)

# Crashpad Headers
include_directories(${PROJECT_SOURCE_DIR}/crashpad/include/ ${PROJECT_SOURCE_DIR}/crashpad/include/third_party/mini_chromium/mini_chromium/)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                        crashpad_client_common
                        crashpad_client
                        crashpad_util
                        base
        )
```

4. In [AndroidManifest.xml](https://github.com/BugSplat-Git/my-android-crasher/blob/master/app/src/main/AndroidManifest.xml), add permissions for INTERNET and READ_EXTERNAL_STORAGE and set extractNativeLibs="true" 

```xml
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
<application
        android:extractNativeLibs="true"
        ...>
```

5. Initialize Crashpad in your native library.

```cpp
extern "C" JNIEXPORT jboolean JNICALL
Java_com_bugsplat_my_1android_1crasher_MainActivity_initializeCrashpad(
        JNIEnv* env,
        jobject /* this */,
        jstring appDataDir,
        jstring libDir
    ) {
    // Device file paths
    string nativeLibraryDir = env->GetStringUTFChars(libDir, nullptr);
    string dataDir = env->GetStringUTFChars(appDataDir, nullptr);

    // Crashpad file paths
    FilePath handler(nativeLibraryDir + "/libcrashpad_handler.so");
    FilePath reportsDir(dataDir + "/crashpad");
    FilePath metricsDir(dataDir + "/crashpad");

    // Crashpad upload URL for BugSplat database
    string database = BUGSPLAT_DATABASE;
    string url = "https://" + database + ".bugsplat.com/post/bp/crash/crashpad.php";

    // Crashpad annotations
    map<string, string> annotations;
    annotations["format"] = "minidump";             // Required: Crashpad setting to save crash as a minidump
    annotations["database"] = database;             // Required: BugSplat database
    annotations["product"] = "my-android-crasher";  // Required: BugSplat appName
    annotations["version"] = "1.0.1";               // Required: BugSplat appVersion
    annotations["key"] = "key!";                    // Optional: BugSplat key field
    annotations["user"] = "fred@bugsplat.com";      // Optional: BugSplat user email
    annotations["list_annotations"] = "Sample comment"; // Optional: BugSplat crash description

    // Crashpad arguments
    vector<string> arguments;
    arguments.emplace_back("--no-rate-limit");

    // Crashpad local database
    unique_ptr<CrashReportDatabase> crashReportDatabase = CrashReportDatabase::Initialize(reportsDir);
    if (crashReportDatabase == nullptr) return false;

    // Enable automated crash uploads
    Settings *settings = crashReportDatabase->GetSettings();
    if (settings == nullptr) return false;
    settings->SetUploadsEnabled(true);

    // File paths of attachments to be uploaded with the minidump file at crash time - default bundle limit is 20MB
    vector<FilePath> attachments;
    FilePath attachment(dataDir + "/files/attachment.txt");
    attachments.push_back(attachment);

    // Start Crashpad crash handler
    auto *client = new CrashpadClient();
    bool status = client->StartHandlerAtCrash(
        handler,
        reportsDir,
        metricsDir,
        url,
        annotations,
        arguments,
        attachments
    );
    return status;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_bugsplat_my_1android_1crasher_MainActivity_crash(
        JNIEnv* env,
        jobject /* this */) {

    stackFrame1();

    return true;
}

void stackFrame1() {
    stackFrame2();
}

void stackFrame2() {
    stackFrame3();
}

void stackFrame3() {
    crash();
}

void crash() {
    *(volatile int *)nullptr = 0;
}
```

6. Add code that will allow you to trigger a native crash.

```kotlin
public fun btnCrashClick(view: View) {
    // Example of a call to a native method
    crash()
}

/**
* A native method that is implemented by the 'native-lib' native library,
* which is packaged with this application.
*/
external fun initializeCrashpad(dataDir: String, nativeLibraryDir: String): Boolean
external fun crash(): Boolean
```
