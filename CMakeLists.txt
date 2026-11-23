cmake_minimum_required(VERSION 3.16)
project(AgenticGameEngine
    VERSION 0.1.0
    DESCRIPTION "A modern, lightweight C++ OpenGL game engine designed for 2D+ and submodule embedding"
    LANGUAGES C CXX
)

# -----------------------------------------------------------------------------
# 1. Standard & Build Settings
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check if this project is being built standalone or consumed as a submodule
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(ENGINE_IS_TOP_LEVEL ON)
else()
    set(ENGINE_IS_TOP_LEVEL OFF)
endif()

option(ENGINE_BUILD_TESTS "Build unit tests" ${ENGINE_IS_TOP_LEVEL})
option(ENGINE_BUILD_DEMOS "Build demo applications" ${ENGINE_IS_TOP_LEVEL})

# -----------------------------------------------------------------------------
# 2. Dependencies (Option A: System package with FetchContent fallback)
# -----------------------------------------------------------------------------
include(FetchContent)

# OpenGL
find_package(OpenGL QUIET)
if(NOT OpenGL_FOUND)
    message(WARNING "[Engine] OpenGL development package not found on system. Looking for runtime libraries...")
    find_library(OPENGL_LIB NAMES GL OpenGL PATHS /usr/lib/x86_64-linux-gnu /usr/lib)
endif()

# GLFW (Windowing & Context)
option(ENGINE_ENABLE_WINDOWING "Enable windowing and input via GLFW" ON)

if(ENGINE_ENABLE_WINDOWING)
    find_package(glfw3 3.3 QUIET)
    if(NOT glfw3_FOUND)
        # Check if X11 development headers required to build GLFW from source exist
        find_package(X11 QUIET COMPONENTS Xrandr Xinerama Xcursor Xi)
        if(X11_Xrandr_FOUND)
            message(STATUS "[Engine] GLFW3 not found on system. Fetching via FetchContent...")
            FetchContent_Declare(
                glfw
                GIT_REPOSITORY https://github.com/glfw/glfw.git
                GIT_TAG 3.3.8
                GIT_SHALLOW TRUE
            )
            set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
            set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
            set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
            set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
            FetchContent_MakeAvailable(glfw)
        else()
            message(WARNING "[Engine] X11/RandR development packages missing on host. Disabling GLFW windowing until 'sudo apt install -y libglfw3-dev libgl-dev' is installed.")
            set(ENGINE_ENABLE_WINDOWING OFF CACHE BOOL "" FORCE)
        endif()
    endif()
endif()

# -----------------------------------------------------------------------------
# 3. Engine Target Definition
# -----------------------------------------------------------------------------
add_library(engine STATIC
    src/engine/core/log.cpp
    src/engine/core/version.cpp
)

# Aliasing allows downstream submodule consumers and internal targets to use:
# target_link_libraries(MyTarget PRIVATE engine::engine)
add_library(engine::engine ALIAS engine)

# Header encapsulation:
# Public include directory is strictly for clean public API in include/
# Internal headers in src/ remain private to the engine
target_include_directories(engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Compiler warnings
if(MSVC)
    target_compile_options(engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Linking dependencies
if(TARGET OpenGL::GL)
    target_link_libraries(engine PUBLIC OpenGL::GL)
elseif(OPENGL_LIB)
    target_link_libraries(engine PUBLIC ${OPENGL_LIB} ${CMAKE_DL_LIBS})
else()
    target_link_libraries(engine PUBLIC ${CMAKE_DL_LIBS})
endif()

if(TARGET glfw)
    target_link_libraries(engine PRIVATE glfw)
endif()

# -----------------------------------------------------------------------------
# 4. Tests & Demos (Gated for Submodule Consumption)
# -----------------------------------------------------------------------------
if(ENGINE_BUILD_TESTS)
    enable_testing()
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        message(STATUS "[Engine] Google Test not found on system. Fetching via FetchContent...")
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
            GIT_SHALLOW TRUE
        )
        set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()
    add_subdirectory(tests)
endif()

if(ENGINE_BUILD_DEMOS)
    add_subdirectory(demos)
endif()
