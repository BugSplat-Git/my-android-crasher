# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp )

# Crashpad Libraries
add_library(crashpad_common STATIC IMPORTED)
set_property(TARGET crashpad_common PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/client/libcommon.a)

add_library(crashpad_client STATIC IMPORTED)
set_property(TARGET crashpad_client PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/client/libclient.a)

add_library(crashpad_util STATIC IMPORTED)
set_property(TARGET crashpad_util PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/util/libutil.a)

add_library(base STATIC IMPORTED)
set_property(TARGET base PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/base/libbase.a)

# Crashpad Handler
add_library(crashpad_handler SHARED IMPORTED)
set_property(TARGET crashpad_handler PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/crashpad/lib/${ANDROID_ABI}/libcrashpad_handler.so)

# Crashpad Headers
include_directories(${PROJECT_SOURCE_DIR}/crashpad/include/ ${PROJECT_SOURCE_DIR}/crashpad/include/third_party/mini_chromium/mini_chromium/)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log
            )


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                        crashpad_common
                        crashpad_client
                        crashpad_util
                        base
        )

# Disable optimizations for debugging
target_compile_options(native-lib PRIVATE
        "$<$<CONFIG:RELEASE>:-O0>"
        "$<$<CONFIG:DEBUG>:-O0>"
)