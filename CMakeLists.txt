# plugify-source2
# Copyright (C) 2023-2025 untrustedmodders
# Licensed under the GPLv3 license. See LICENSE file in the project root for details.

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()


file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" VERSION_FILE_CONTENTS)
string(STRIP "${VERSION_FILE_CONTENTS}" VERSION_FILE_CONTENTS)
set(S2SDK_VERSION "${VERSION_FILE_CONTENTS}" CACHE STRING "Set version name")
set(S2SDK_PACKAGE "s2sdk" CACHE STRING "Set package name")
string(REPLACE "v" "" S2SDK_VERSION "${S2SDK_VERSION}")
string(REGEX REPLACE "[.+-]" ";" S2SDK_VERSION_LIST ${S2SDK_VERSION})
list(GET S2SDK_VERSION_LIST 0 S2SDK_VERSION_MAJOR)
list(GET S2SDK_VERSION_LIST 1 S2SDK_VERSION_MINOR)
list(GET S2SDK_VERSION_LIST 2 S2SDK_VERSION_PATCH)

project(plugify-plugin-s2sdk
        VERSION "${S2SDK_VERSION_MAJOR}.${S2SDK_VERSION_MINOR}.${S2SDK_VERSION_PATCH}"
        DESCRIPTION "Source2 SDK Plugin"
        HOMEPAGE_URL "https://github.com/untrustedmodders/plugify-plugin-s2sdk"
        LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE MATCHES "Debug|Devel|MinSizeRel|RelWithDebInfo|Release")
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Release.")
    set(CMAKE_BUILD_TYPE Release)
endif()

if(UNIX)
    if(APPLE)
        set(MACOS TRUE)
    else()
        set(LINUX TRUE)
    endif()
endif()

if(WIN32)
    if(NOT MSVC)
        message(FATAL_ERROR "MSVC restricted")
    endif()

    set(WINDOWS TRUE)
endif()

string(REGEX REPLACE "^[^-]*-" "" PROJECT_NAME_SUBSTRING "${PROJECT_NAME}")
string(TOLOWER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_LOWER)
string(TOUPPER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_UPPER)

#
# Plugufy
#
add_subdirectory(external/configs)

#
# Dynlibutils
#
add_subdirectory(external/dynlibutils)

#
# Maps
#
add_subdirectory(external/parallel-hashmap)
include_directories(external/parallel-hashmap)

#
# V8
#
set(V8_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/v8/include/")
if(WIN32)
    set(V8LM_LINK_LIBRARIES winmm dbghelp)
    set(V8_COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS=1)
elseif(LINUX)
    set(V8LM_LINK_LIBRARIES pthread)
endif()
add_library(v8_monolith SHARED IMPORTED)
if(WIN32)
    set(V8LM_V8_RELEASE_LIB "${CMAKE_CURRENT_SOURCE_DIR}/v8/libs/windows/v8.dll")
    set(V8LM_V8_RELEASE_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/v8/libs/windows/v8.lib")

    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION ${V8LM_V8_RELEASE_LIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_IMPLIB ${V8LM_V8_RELEASE_IMPLIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION_RELEASE  ${V8LM_V8_RELEASE_LIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_IMPLIB_RELEASE ${V8LM_V8_RELEASE_IMPLIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION_RELWITHDEBINFO ${V8LM_V8_RELEASE_LIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_IMPLIB_RELWITHDEBINFO ${V8LM_V8_RELEASE_IMPLIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION_MINSIZEREL ${V8LM_V8_RELEASE_LIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_IMPLIB_MINSIZEREL ${V8LM_V8_RELEASE_IMPLIB})
elseif(LINUX)
    set(V8LM_V8_RELEASE_LIB "${CMAKE_CURRENT_SOURCE_DIR}/v8/libs/linux/libv8.so")

    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION ${V8LM_V8_RELEASE_LIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION_RELEASE ${V8LM_V8_RELEASE_LIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION_RELWITHDEBINFO ${V8LM_V8_RELEASE_LIB})
    set_property(TARGET v8_monolith PROPERTY IMPORTED_LOCATION_MINSIZEREL ${V8LM_V8_RELEASE_LIB})
endif()
target_include_directories(v8_monolith INTERFACE ${V8_INCLUDE_DIRS})
target_link_libraries(v8_monolith INTERFACE ${V8LM_LINK_LIBRARIES})
if(MSVC)
    target_compile_options(v8_monolith INTERFACE /wd4100 /wd4244)
else()
    target_compile_options(v8_monolith INTERFACE -Wno-unused-parameter -Wno-conversion -Wno-template-id-cdtor -Wno-shadow)
endif()

#
# SourceSDK
#
set(SOURCESDK_LINK_STRIP_CPP_EXPORTS ON CACHE INTERNAL "")
set(SOURCESDK_LINK_STEAMWORKS ON CACHE INTERNAL "")
set(SOURCESDK_MALLOC_OVERRIDE ON CACHE INTERNAL "")
set(SOURCESDK_CONFIGURE_EXPORT_MAP OFF CACHE INTERNAL "")

set(SOURCESDK_GAME_TARGET "cs2" CACHE INTERNAL "Set game library name to start after")
if(SOURCESDK_GAME_TARGET STREQUAL "deadlock")
    set(SOURCESDK_PROTOS gcsdk_gcmessages networkbasetypes network_connection CACHE INTERNAL "")
endif()

add_subdirectory(external/sourcesdk)

set(S2SDK_GAME_NAME "csgo" CACHE INTERNAL "Set game name")
set(S2SDK_COMPILE_DEFINITIONS
        S2SDK_VERSION="${S2SDK_VERSION}"
        S2SDK_PACKAGE="${S2SDK_PACKAGE}"
        S2SDK_GAME_NAME="${S2SDK_GAME_NAME}"

        S2SDK_LIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}"
        S2SDK_LIBRARY_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"

        PLUGIFY_FORMAT_SUPPORT=$<BOOL:${COMPILER_SUPPORTS_FORMAT}>
        PLUGIFY_IS_DEBUG=$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>
        PLUGIFY_ARCH_BITS=$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,64,32>
)

if(WINDOWS)
    set(S2SDK_COMPILE_DEFINITIONS
            ${S2SDK_COMPILE_DEFINITIONS}
            S2SDK_PLATFORM="win64"
            S2SDK_ROOT_BINARY="bin/win64/"
            S2SDK_GAME_BINARY="csgo/bin/win64/"
            NOMINMAX=1
    )
elseif(LINUX)
    set(S2SDK_COMPILE_DEFINITIONS
            ${S2SDK_COMPILE_DEFINITIONS}
            S2SDK_PLATFORM="linuxsteamrt64"
            S2SDK_ROOT_BINARY="bin/linuxsteamrt64/"
            S2SDK_GAME_BINARY="bin/linuxsteamrt64/../../csgo/bin/linuxsteamrt64/" # Metamod hack
    )
endif()

file(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES}
        src/core/sdk/helpers.hpp
        src/export/bodies.cpp
        src/export/models.cpp
        src/export/debug.cpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp")

set_target_properties(${PROJECT_NAME} PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF

        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

if(WINDOWS)
    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(MACOS)
    set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES "x86_64")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE sourcesdk plugify-plugin-configs cpp-memory_utils v8_monolith)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /Zc:preprocessor $<$<CONFIG:Release>:/Zi>)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/DEBUG /OPT:REF /OPT:ICF>)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wextra -Wshadow -Wconversion $<$<CONFIG:Release>:-g1>) # -Werror -Wpedantic
endif()

if(MACOS)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/sym/exported_symbols.lds")
elseif(LINUX)
    target_link_options(${PROJECT_NAME} PRIVATE
            #-Wl,--wrap=_ZNSs12_S_empty_repEv
            #-Wl,--wrap=_ZNSbIwSt11char_traitsIwESaIwEE12_S_empty_repEv
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/sym/version_script.lds")
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_MACRO_NAME PLUGIN_API EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/plugin_export.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/exports)

if(LINUX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLIBCXX_USE_CXX11_ABI=0) # for protobuf
    target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
endif()

#target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${S2SDK_COMPILE_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PRIVATE ${S2SDK_COMPILE_OPTIONS})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/plugify/include")

if(NOT COMPILER_SUPPORTS_FORMAT)
    #target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
endif()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/plugify-plugin-s2sdk.pplugin.in
        ${CMAKE_CURRENT_BINARY_DIR}/plugify-plugin-s2sdk.pplugin.json
)

execute_process(
        COMMAND python3 -c "import json,sys; print(json.dumps(json.load(sys.stdin), separators=(',',':')))"
        INPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/plugify-plugin-s2sdk.pplugin.json
        OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/plugify-plugin-s2sdk.pplugin
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/gamedata.jsonc
        ${CMAKE_CURRENT_BINARY_DIR}/gamedata.jsonc
        COPYONLY
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/settings.jsonc
        ${CMAKE_CURRENT_BINARY_DIR}/settings.jsonc
        COPYONLY
)
