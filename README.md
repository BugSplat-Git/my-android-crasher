[![BugSplat](https://s3.amazonaws.com/bugsplat-public/npm/header.png)](https://www.bugsplat.com)

# AndroidCrasher

### Overview
This repository contains an Android NDK library integrated with [Crashpad](https://chromium.googlesource.com/crashpad/crashpad/+/master/README.md) and [BugSplat](https://www.bugsplat.com). This repository includes pre-built versions of the Crashpad libraries as well as the Breakpad tools [dump_syms](https://github.com/google/breakpad/tree/master/src/tools/linux/dump_syms) and [symupload](https://github.com/google/breakpad/tree/master/src/tools/linux/symupload) that are used to generate [.sym](https://chromium.googlesource.com/breakpad/breakpad/+/master/docs/processor_design.md#symbol-files) files so minidumps can be symbolicated by BugSplat.

### Steps
1. Install Android Studio and create an Android emulator
2. Clone this repository `git clone https://github.com/BugSplat-Git/AndroidCrasher.git`
3. Build the project, this should cause the Android NDK to be installed if it isn't installed already
4. Run the [symupload script](https://github.com/BugSplat-Git/AndroidCrasher/tree/master/tools) that corresponds to your development system's OS, this is required in order to get function names and line numbers in your crash reports
5. Run the application without making any code changes so the project isn't rebuilt, the application will crash and generate a report automatically
6. Navigate to BugSplat's [Crashes](https://app.bugsplat.com/v2/crashes?database=Fred&c0=appName&f0=CONTAINS&v0=AndroidCrasher) page to see a list of recent AndroidCrasher crashes
7. Click the link in the ID column to see full debug information for the generated crash
8. Upload new symbols every time you rebuild the application otherwise you will not see function names or line numbers in the crash report's stack trace

### Additional Considerations
The Breakpad tool dump_syms was built on a Linux system and needs to run on a compatible system (or subsystem) in order to create the symbol files. This repository includes 3 methods for running dump_syms depending on what system you're using to build the Android project. For Windows, run the dump_syms_windows.ps1 script at build time which uses [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) to run dump_syms. On Mac OS X, run dump_syms_osx.sh which uses [linux-noah](https://github.com/linux-noah/noah) (which can be installed by Homebrew) to run dump_syms. On Linux, dump_syms_linux.sh can be invoked directly at build time.
