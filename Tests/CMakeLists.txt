#==============================================================================
# ShowMIDI Test Suite
# Feature: 004-tdd-adoption
# Framework: JUCE UnitTest (juce_core/unit_tests)
#==============================================================================

cmake_minimum_required(VERSION 3.15)

# Create test console application
juce_add_console_app(ShowMIDI_Tests
    PRODUCT_NAME "ShowMIDI Tests"
)

# Generate JUCE header for test target
juce_generate_juce_header(ShowMIDI_Tests)

# Test source files (will be populated as tests are added)
# Phase 0 (Week 1): Baseline tests
# Phase 1 (Weeks 2-3): Expand unit tests
# Phase 2 (Weeks 4-6): Plugin and integration tests
# Phase 3 (Weeks 7-8): Platform-specific tests

# Adapter fixtures (Phase 2 Foundational)
set(FIXTURE_FILES
    Main.cpp
    Fixtures/MockMidiAdapter.cpp
    Fixtures/SimulatedTimeProvider.cpp
)

# Test files (Phase 3+: Example and production tests)
set(TEST_FILES
    Unit/ExampleTest.cpp
    # Additional unit tests will go here (e.g., Unit/ThemeTests.cpp)
    # Integration tests will go here (e.g., Integration/MidiFlowTests.cpp)
    # System tests will go here (e.g., System/StandaloneAppTests.cpp)
)

target_sources(ShowMIDI_Tests PRIVATE ${FIXTURE_FILES} ${TEST_FILES})

# Enable JUCE UnitTest framework
target_compile_definitions(ShowMIDI_Tests PRIVATE
    JUCE_UNIT_TESTS=1
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_STANDALONE_APPLICATION=1
)

# Link JUCE modules (same as main ShowMIDI plugin)
target_link_libraries(ShowMIDI_Tests PRIVATE
    juce::juce_core
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Compiler warnings as errors (consistent with main project)
target_compile_options(ShowMIDI_Tests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Werror>
)

# CTest integration for CI/CD
# Register tests with labels for selective execution

# Phase 4: Example test registration (more tests added in Phase 5+)
add_test(NAME ExampleTest COMMAND ShowMIDI_Tests --category Tutorial)
set_tests_properties(ExampleTest PROPERTIES
    LABELS "unit;tutorial;fast"
    COST 5
    TIMEOUT 300
)

# Cost-based scheduling (unit < integration < system)
# Fast tests (unit): COST 5-10 (run first)
# Medium tests (integration): COST 20-40
# Slow tests (system): COST 50-100

# Label categories:
# - Scope: unit, integration, system
# - Domain: midi, ui, settings, plugin
# - Speed: fast (<100ms), medium (<500ms), slow (<5s)
# - Platform: ios, plugin-au, plugin-vst3, plugin-lv2
