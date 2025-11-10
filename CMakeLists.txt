cmake_minimum_required(VERSION 3.15)

# To build a CLAP you need to do the following in addition to editing this file
#
# - First do the native build of at least the ShowMIDI_shared target on your platform as Debug or Release
# - Then do `cmake -Bbuild_clap -DCMAKE_BUILD_TYPE=Debug` or Release of course
# - Then do `cmake --build build_clap --config Debug` or Release
# 
# This will result in build_clap/ShowMIDI_artefacts/Debug/ShowMIDI.clap (or Release).
#
# For this to work a couple of things need to be configured

# JUCE path resolution with priority: PATH_TO_JUCE env var → submodule → FATAL_ERROR
if(DEFINED ENV{PATH_TO_JUCE})
    set(PATH_TO_JUCE "$ENV{PATH_TO_JUCE}")
    message(STATUS "Using JUCE from PATH_TO_JUCE environment variable: ${PATH_TO_JUCE}")
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    set(PATH_TO_JUCE "${CMAKE_CURRENT_SOURCE_DIR}/JUCE")
    message(STATUS "Using JUCE from submodule: ${PATH_TO_JUCE}")
else()
    message(FATAL_ERROR "JUCE not found. Please either:\n"
        "  1. Set PATH_TO_JUCE environment variable to point to your JUCE installation, OR\n"
        "  2. Initialize the JUCE submodule: git submodule update --init --recursive")
endif()

# CLAP extensions - OPTIONAL dependency
# CLAP is an optional plugin format. If the clap-juce-extensions submodule is not initialized,
# the build will continue successfully but skip the CLAP plugin format with a warning.
# To enable CLAP: git submodule update --init --recursive
set(PATH_TO_CLAP_EXTENSIONS "${CMAKE_CURRENT_SOURCE_DIR}/libs/clap-juce-extensions")

if(EXISTS "${PATH_TO_CLAP_EXTENSIONS}/cmake/JucerClap.cmake")
    message(STATUS "CLAP extensions found at: ${PATH_TO_CLAP_EXTENSIONS}")
    set(BUILD_CLAP ON)
else()
    message(WARNING "CLAP extensions not found at ${PATH_TO_CLAP_EXTENSIONS}. CLAP plugin format will be skipped.\n"
        "  To enable CLAP: git submodule update --init --recursive")
    set(BUILD_CLAP OFF)
endif()

# Make sure to set the same C++ version as you have set in the Projucer
set(CMAKE_CXX_STANDARD 17)

# Make sure to set the same MacOS deployment target as you have set in the Projucer
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.7" CACHE STRING "Minimum OS X deployment target")

# If the Projucer is using "static runtime" for Visual Studio:
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Runtime")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded" CACHE STRING "Runtime")

# This bypasses XCode signing and is appropriate if you sign after a build. Comment it out if you want to use 
# xcode signing.
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")

project(ShowMIDI VERSION 1.1.1)

# define the exporter types used in your Projucer configuration
if (APPLE)
    set(JUCER_GENERATOR "Xcode")
elseif (WIN32)
    set(JUCER_GENERATOR "VisualStudio2022")
else () # Linux
    set(JUCER_GENERATOR "LinuxMakefile")
endif ()

# Conditionally build CLAP plugin format (only if clap-juce-extensions available)
if(BUILD_CLAP)
    include(${PATH_TO_CLAP_EXTENSIONS}/cmake/JucerClap.cmake)
    create_jucer_clap_target(
            TARGET ShowMIDI 
            PLUGIN_NAME "ShowMIDI"
            BINARY_NAME "ShowMIDI" 

            # You will want to check all this information
            MANUFACTURER_NAME "Uwyn"
            MANUFACTURER_CODE Uwyn
            PLUGIN_CODE shmi
            VERSION_STRING "$ENV{RELEASE_VERSION}"
            CLAP_ID "com.uwyn.showmidi.clap"
            CLAP_FEATURES note-effect utility
            CLAP_MANUAL_URL "https://www.uwyn.com"
            CLAP_SUPPORT_URL "https://www.uwyn.com"
            EDITOR_NEEDS_KEYBOARD_FOCUS TRUE
    )
endif()

