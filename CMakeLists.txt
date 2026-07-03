cmake_minimum_required(VERSION 3.15)

# Setup libKCD1
set(LIBKCD1_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/extern/libKCD1")
set(LIBKCD1_BUILDENV "${LIBKCD1_ROOT}/.buildenv")
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()
set(RE_ROOT "${LIBKCD1_ROOT}")
set(RE_BUILDENV "${LIBKCD1_BUILDENV}")

# Use project-root manifest for vcpkg (includes libKCD1 deps + project-specific deps).
set(VCPKG_MANIFEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "" FORCE)
set(VCPKG_MANIFEST_MODE ON CACHE BOOL "" FORCE)

# Init Project
set(PROJECT_NAME ToggleHud)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("include")

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(boost_container CONFIG REQUIRED)
find_package(SimpleIni CONFIG REQUIRED)

file(GLOB_RECURSE RE_HEADERS "${RE_ROOT}/include/*.h")
file(GLOB_RECURSE RE_SOURCES "${RE_ROOT}/src/*.cpp")

# kcd_re
add_library(kcd_re STATIC
    "${RE_BUILDENV}/check_sizes.cpp"
    "${RE_BUILDENV}/CrySDKStubs.cpp"
    ${RE_SOURCES}
    ${RE_HEADERS}
)
target_precompile_headers(kcd_re PRIVATE "${RE_BUILDENV}/PCH.h")
target_compile_definitions(kcd_re PRIVATE _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)
target_compile_options(kcd_re PRIVATE /utf-8 /W4 /wd4819 /wd4100)
target_include_directories(kcd_re PRIVATE
    ${RE_ROOT}/include
    ${RE_ROOT}/include/CryEngine/CryCommon
)
target_link_libraries(kcd_re PUBLIC Boost::container)

# ToggleHud
file(GLOB_RECURSE PLUGIN_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${PLUGIN_HEADERS} ${PLUGIN_SOURCES})

target_precompile_headers(${PROJECT_NAME} PRIVATE "${RE_BUILDENV}/PCH.h")
target_compile_definitions(${PROJECT_NAME} PRIVATE _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)
target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /W4 /wd4819 /wd4100)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${RE_ROOT}/include
    ${RE_ROOT}/include/CryEngine/CryCommon
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    $<TARGET_PROPERTY:spdlog::spdlog_header_only,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL_HO SPDLOG_FWRITE_UNLOCKED)
target_link_libraries(${PROJECT_NAME} PRIVATE kcd_re fmt::fmt-header-only SimpleIni::SimpleIni)

# Move INI to output automatically
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/package/ToggleHud.ini"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ToggleHud.ini"
)