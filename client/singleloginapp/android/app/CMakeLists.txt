# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(GRPC_SRC_DIR ../../../../deps/grpc/)

set(GRPC_BUILD_DIR ../grpc/outputs/${ANDROID_ABI})
file(MAKE_DIRECTORY ${GRPC_BUILD_DIR})


set(helloworld_PROTOBUF_PROTOC_EXECUTABLE "/usr/local/bin/protoc" CACHE STRING "Protoc binary on host")
set(helloworld_GRPC_CPP_PLUGIN_EXECUTABLE "/usr/local/bin/grpc_cpp_plugin" CACHE STRING "gRPC CPP plugin binary on host")


add_subdirectory(${GRPC_SRC_DIR} ${GRPC_BUILD_DIR})

set(GRPC_PROTO_GENS_DIR ${CMAKE_BINARY_DIR}/gens)
file(MAKE_DIRECTORY ${GRPC_PROTO_GENS_DIR})
include_directories(${GRPC_PROTO_GENS_DIR})

function(android_protobuf_grpc_generate_cpp SRC_FILES HDR_FILES INCLUDE_ROOT)
    if(NOT ARGN)
        message(SEND_ERROR "Error: android_protobuf_grpc_generate_cpp() called without any proto files")
        return()
    endif()

    set(${SRC_FILES})
    set(${HDR_FILES})
    set(PROTOBUF_INCLUDE_PATH -I ${INCLUDE_ROOT})
    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        file(RELATIVE_PATH REL_FIL ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_ROOT} ${ABS_FIL})
        get_filename_component(REL_DIR ${REL_FIL} DIRECTORY)
        set(RELFIL_WE "${REL_DIR}/${FIL_WE}")

        list(APPEND ${SRC_FILES} "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.cc")
        list(APPEND ${HDR_FILES} "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.h")
        list(APPEND ${SRC_FILES} "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.cc")
        list(APPEND ${HDR_FILES} "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.h")

        add_custom_command(
                OUTPUT "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.cc"
                "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.h"
                "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.cc"
                "${GRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.h"
                COMMAND ${helloworld_PROTOBUF_PROTOC_EXECUTABLE}
                ARGS --grpc_out=${GRPC_PROTO_GENS_DIR}
                --cpp_out=${GRPC_PROTO_GENS_DIR}
                --plugin=protoc-gen-grpc=${helloworld_GRPC_CPP_PLUGIN_EXECUTABLE}
                ${PROTOBUF_INCLUDE_PATH}
                ${INCLUDE_ROOT}/${REL_FIL}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${helloworld_PROTOBUF_PROTOC_EXECUTABLE} ${helloworld_GRPC_CPP_PLUGIN_EXECUTABLE} ${ABS_FIL} )
    endforeach()

    set_source_files_properties(${${SRC_FILES}} ${${HDR_FILES}} PROPERTIES GENERATED TRUE)
    set(${SRC_FILES} ${${SRC_FILES}} PARENT_SCOPE)
    set(${HDR_FILES} ${${HDR_FILES}} PARENT_SCOPE)
endfunction()

set(PROTO_BASE_DIR ../../djinni/protos/)

android_protobuf_grpc_generate_cpp(
        HELLOWORLD_PROTO_SRCS HELLOWORLD_PROTO_HDRS ${PROTO_BASE_DIR} ${PROTO_BASE_DIR}/account.proto)

add_library(helloworld_proto_lib
        SHARED ${HELLOWORLD_PROTO_HDRS} ${HELLOWORLD_PROTO_SRCS})

target_link_libraries(helloworld_proto_lib
        grpc++
        libprotobuf
        android
        log)

include_directories(
#        /usr/include/
#        /usr/include/x86_64-linux-gnu/
#        ../../djinni/grpc/src/core/lib/json/

        ../../djinni/deps/json/
        ../../djinni/deps/support-lib/
        ../../djinni/deps/support-lib/jni/
        ../../djinni/generated-src/cpp/
        ../../djinni/generated-src/jni/
        ../../djinni/authso/

        ../../djinni/authso/
        ../../djinni/authso/utils/
        ../../djinni/authso/model/
        ../../djinni/authso/struct/
        ../../djinni/authso/storage/
        ../../djinni/authso/network/
#        ../../djinni/grpc/include/grpc++
)

file(GLOB login_sources
#        /usr/include/nl_types.h
#        /usr/include/x86_64-linux-gnu/gnu/stubs.h
#        /usr/include/x86_64-linux-gnu/gnu/stubs-32.h

#        ../../djinni/deps/json/*.inl
        ../../djinni/deps/json/json.hpp
#        ../../djinni/deps/json/*.cpp
#        ../../djinni/grpc/src/core/lib/json/*.cc
#        ../../djinni/grpc/src/core/lib/json/*.h

        ../../djinni/deps/support-lib/jni/*.hpp
        ../../djinni/deps/support-lib/jni/*.cpp
        ../../djinni/generated-src/jni/*.hpp
        ../../djinni/generated-src/jni/*.cpp
        src/main/cpp/*.h
        src/main/cpp/*.cpp

        ../../djinni/authso/*.hpp
        ../../djinni/authso/*.h
        ../../djinni/authso/*.cpp
        ../../djinni/authso/utils/*.h
        ../../djinni/authso/utils/*.cpp
        ../../djinni/authso/model/*.h
        ../../djinni/authso/model/*.cpp
        ../../djinni/authso/struct/*.h
        ../../djinni/authso/struct/*.cpp
        ../../djinni/authso/storage/*.h
        ../../djinni/authso/storage/*.cpp
        ../../djinni/authso/network/*.h
        ../../djinni/authso/network/*.cpp
        )

add_library( # Sets the name of the library.
        authso

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${login_sources}
        )

target_include_directories(authso
        PRIVATE ${HELLOWORLD_PROTO_HEADERS})

target_link_libraries(authso
        helloworld_proto_lib
        android
        log)
