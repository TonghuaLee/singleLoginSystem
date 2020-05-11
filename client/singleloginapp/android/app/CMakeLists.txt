# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        ../../djinni/deps/json/
        ../../djinni/deps/support-lib/
        ../../djinni/deps/support-lib/jni/
        ../../djinni/generated-src/cpp/
//        ../../djinni/generated-src/jni/
        ../../djinni/authso/

#        ../../djinni/
#        ../../djinni/utils
#        ../../djinni/model
#        ../../djinni/struct
#        ../../djinni/storage
#        ../../djinni/network
)


file(GLOB authoso_sources

        ../../djinni/deps/support-lib/jni/*.hpp
        ../../djinni/deps/support-lib/jni/*.cpp
        ../../djinni/generated-src/jni/*.hpp
        ../../djinni/generated-src/jni/*.cpp

        ../../djinni/authso/*.hpp
        ../../djinni/authso/*.h
        ../../djinni/authso/*.cpp
#        ../../djinni/utils/*.h
#        ../../djinni/utils/*.cpp
#        ../../djinni/model/*.h
#        ../../djinni/model/*.cpp
#        ../../djinni/struct/*.h
#        ../../djinni/struct/*.cpp
#        ../../djinni/storage/*.h
#        ../../djinni/storage/*.cpp
#        ../../djinni/network/*.h
#        ../../djinni/network/*.cpp
        )

add_library( # Sets the name of the library.
        authso

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${authoso_sources}
        )


target_link_libraries(
        authso
        log)

