cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DynamicResonator VERSION 1.3.0)

# --- JUCE framework -----------------------------------------------------------
set(PNEUTRINO_JUCE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/JUCE"
    CACHE PATH "Path to the pinned JUCE source checkout")
if(NOT EXISTS "${PNEUTRINO_JUCE_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "JUCE is missing. Run git submodule update --init --recursive, "
        "or set PNEUTRINO_JUCE_SOURCE_DIR to an existing matching checkout.")
endif()
add_subdirectory("${PNEUTRINO_JUCE_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/JUCE")

# --- Plugin target ------------------------------------------------------------
juce_add_plugin(DynamicResonator
    VERSION           1.3.0
    COMPANY_NAME      "NeutronStar714"
    IS_SYNTH          FALSE
    NEEDS_MIDI_INPUT  FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    PLUGIN_MANUFACTURER_CODE DRes
    PLUGIN_CODE       DynR
    FORMATS           VST3 Standalone
    PRODUCT_NAME      "Dynamic Resonator"
    BUNDLE_ID         "com.dynamicresonator.DynamicResonator"
)

juce_generate_juce_header(DynamicResonator)

# --- Sources ------------------------------------------------------------------
target_sources(DynamicResonator PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

# --- Compile definitions ------------------------------------------------------
target_compile_definitions(DynamicResonator PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# --- Linked libraries ---------------------------------------------------------
target_link_libraries(DynamicResonator PRIVATE
    juce::juce_dsp
    juce::juce_audio_utils
    juce::juce_audio_devices
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
