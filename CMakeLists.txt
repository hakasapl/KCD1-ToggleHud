cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME togglehud)

# Keep libKCD1 paths available before project() so vcpkg toolchain can read them.
set(LIBKCD1_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/extern/libKCD1")
set(LIBKCD1_BUILDENV "${LIBKCD1_ROOT}/.buildenv")

# Mirror libKCD1's expected vcpkg setup when configuring from this repo root.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Use libKCD1's manifest even when configuring from this repository root.
set(VCPKG_MANIFEST_DIR "${LIBKCD1_BUILDENV}" CACHE PATH "" FORCE)
set(VCPKG_MANIFEST_MODE ON CACHE BOOL "" FORCE)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adapt libKCD1's RE_ROOT / RE_BUILDENV variables for this workspace layout.
set(RE_ROOT "${LIBKCD1_ROOT}")
set(RE_BUILDENV "${LIBKCD1_BUILDENV}")

find_package(spdlog CONFIG REQUIRED)
find_package(boost_container CONFIG REQUIRED)

file(GLOB_RECURSE RE_HEADERS "${RE_ROOT}/include/*.h")
file(GLOB_RECURSE RE_SOURCES "${RE_ROOT}/src/*.cpp")

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

file(GLOB_RECURSE PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES})

target_precompile_headers(${PROJECT_NAME} PRIVATE "${RE_BUILDENV}/PCH.h")
target_compile_definitions(${PROJECT_NAME} PRIVATE _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)
target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /W4 /wd4819 /wd4100)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${RE_ROOT}/include
    ${RE_ROOT}/include/CryEngine/CryCommon
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME} PRIVATE kcd_re spdlog::spdlog)