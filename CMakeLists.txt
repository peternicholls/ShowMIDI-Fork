cmake_minimum_required(VERSION 3.15)

project(ShowMIDI VERSION 1.1.3)

# Build configuration options
option(BUILD_TESTS "Build test suite (requires JUCE_UNIT_TESTS=1)" OFF)

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
#
# NOTE: CLAP requires a two-stage build process (see file header comments).
# Temporarily disabled in CI until proper two-stage build is implemented.
set(PATH_TO_CLAP_EXTENSIONS "${CMAKE_CURRENT_SOURCE_DIR}/libs/clap-juce-extensions")

set(BUILD_CLAP OFF)
message(WARNING "CLAP plugin format temporarily disabled. CLAP requires two-stage build (see CMakeLists.txt header).\n"
    "  To enable CLAP locally: Follow instructions in CMakeLists.txt header")

# TODO: Implement proper two-stage CLAP build in CI workflow
# if(EXISTS "${PATH_TO_CLAP_EXTENSIONS}/cmake/JucerClap.cmake")
#     message(STATUS "CLAP extensions found at: ${PATH_TO_CLAP_EXTENSIONS}")
#     set(BUILD_CLAP ON)
# else()
#     message(WARNING "CLAP extensions not found at ${PATH_TO_CLAP_EXTENSIONS}. CLAP plugin format will be skipped.\n"
#         "  To enable CLAP: git submodule update --init --recursive")
#     set(BUILD_CLAP OFF)
# endif()

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

# Linux: Detect required system libraries with clear error messages
# JUCE requires ALSA, X11, and Freetype on Linux. These checks provide actionable error messages
# if dependencies are missing, rather than cryptic JUCE compilation errors.
# MUST be done BEFORE add_subdirectory(JUCE) so juceaide can find FreeType headers
if(UNIX AND NOT APPLE)
    find_package(ALSA REQUIRED)
    if(NOT ALSA_FOUND)
        message(FATAL_ERROR "ALSA development libraries not found.\n"
            "  Install with: sudo apt-get install libasound2-dev")
    endif()
    
    find_package(X11 REQUIRED)
    if(NOT X11_FOUND)
        message(FATAL_ERROR "X11 development libraries not found.\n"
            "  Install with: sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev")
    endif()
    
    find_package(Freetype REQUIRED)
    if(NOT Freetype_FOUND)
        message(FATAL_ERROR "Freetype development libraries not found.\n"
            "  Install with: sudo apt-get install libfreetype6-dev")
    endif()
    
    # Add FreeType2 include directories BEFORE adding JUCE subdirectory
    # This ensures juceaide (built by JUCE) can find ft2build.h
    include_directories(${FREETYPE_INCLUDE_DIRS})
    
    message(STATUS "All required Linux system libraries found (ALSA, X11, Freetype)")
endif()

# define the exporter types used in your Projucer configuration
if (APPLE)
    set(JUCER_GENERATOR "Xcode")
elseif (WIN32)
    set(JUCER_GENERATOR "VisualStudio2022")
else () # Linux
    set(JUCER_GENERATOR "LinuxMakefile")
endif ()

# Add JUCE framework
add_subdirectory(${PATH_TO_JUCE} JUCE)

# Add ShowMIDI plugin target
juce_add_plugin(ShowMIDI
    COMPANY_NAME "Uwyn"
    PLUGIN_MANUFACTURER_CODE Uwyn
    PLUGIN_CODE shmi
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "ShowMIDI"
    BUNDLE_ID com.uwyn.showmidi
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES "Fx" "Tools"
    AU_MAIN_TYPE kAudioUnitType_MIDIProcessor
)

# Override JUCE_PROJUCER_VERSION to match current JUCE version (8.0.10)
# JuceLibraryCode/ is auto-generated by Projucer and may have outdated version
# Since we use CMake exclusively, we override it here to prevent version mismatch errors
target_compile_definitions(ShowMIDI PUBLIC JUCE_PROJUCER_VERSION=0x80010)

# Generate JUCE header
juce_generate_juce_header(ShowMIDI)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
    "Source/*.cpp"
    "Source/*.h"
)

target_sources(ShowMIDI PRIVATE ${SOURCE_FILES})

# Add binary resources (fonts, themes, icons)
juce_add_binary_data(ShowMIDIData SOURCES
    # Fonts
    Fonts/JetBrainsMono-Italic.ttf
    Fonts/JetBrainsMono-Regular.ttf
    Fonts/JetBrainsMono-SemiBold.ttf
    Fonts/JetBrainsMono-SemiBoldItalic.ttf
    # Themes
    "Themes/classic light.svg"
    Themes/bstation.svg
    Themes/darcula.svg
    Themes/dark.svg
    Themes/disco.svg
    Themes/light.svg
    Themes/mouha.svg
    # Icons and assets
    Assets/appicon-ios.png
    Assets/appicon.png
    Assets/bar.svg
    Assets/close.svg
    Assets/collapsed.svg
    Assets/expanded.svg
    Assets/graph.svg
    Assets/help.svg
    Assets/hidden.svg
    Assets/pause.svg
    Assets/play.svg
    Assets/reset.svg
    Assets/settings.svg
    Assets/visible.svg
)

# Link libraries
target_link_libraries(ShowMIDI PRIVATE
    ShowMIDIData
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Compiler definitions
target_compile_definitions(ShowMIDI PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
)

# Compiler warnings (ShowMIDI sources only, not JUCE)
target_compile_options(ShowMIDI PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra>
)

# Enable warnings-as-errors in CI (Code Quality job sets this variable)
if(DEFINED ENV{SHOWMIDI_WERROR})
    target_compile_options(ShowMIDI PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
    )
endif()

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

# Test infrastructure (TDD adoption - feature 004-tdd-adoption)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()
