# -*- CMakeLists.txt generated by CodeLite IDE. Do not edit by hand -*-

cmake_minimum_required(VERSION 2.8.11)

# Project name
project(mx_tune)

# This setting is useful for providing JSON file used by CodeLite for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CONFIGURATION_NAME "cmake")

set(CL_WORKSPACE_DIRECTORY .)
# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${CL_WORKSPACE_DIRECTORY}/cmake-build-${CONFIGURATION_NAME}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

# Projects


# Top project
# Define some variables
set(PROJECT_mx_tune_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(WORKSPACE_PATH "${CMAKE_CURRENT_LIST_DIR}/")



#{{{{ User Code 1
# Place your code here
#}}}}

include_directories(
    .
    .
    JUCE/JuceLibraryCode
    JUCE/Source
    JUCE/JuceLibraryCode/modules
    VST3_SDK

)


# Compiler options
add_definitions(-Os)
add_definitions(-O2)
add_definitions(-std=c++14)
add_definitions(-fpermissive)
add_definitions(-fPIC)
add_definitions(-Wextra)
add_definitions(-Weffc++)
add_definitions(-Woverloaded-virtual)
add_definitions(-Wreorder)
add_definitions(-Wsign-promo)
add_definitions(-Wstrict-null-sentinel)
add_definitions(-Wfloat-equal)
add_definitions(-Wreturn-type)
add_definitions(-Wsign-compare)
add_definitions(-Wsizeof-pointer-memaccess)
add_definitions(-Woverflow)
add_definitions(-Wshadow)
add_definitions(-Wsequence-point)
add_definitions(-Wswitch-default)
add_definitions(-Wunused-but-set-parameter)
add_definitions(-Wunused-but-set-variable)
add_definitions(-Wunused-function)
add_definitions(-Wunused-label)
add_definitions(-Wunused-variable)
add_definitions(-Wmaybe-uninitialized)
add_definitions(-Wuninitialized)
add_definitions(-Wdisabled-optimization)
add_definitions(-Wdelete-non-virtual-dtor)
add_definitions(-Wno-non-virtual-dtor)
add_definitions(-Wno-unused-parameter)

# Linker options
set(LINK_OPTIONS -s)
set(LINK_OPTIONS ${LINK_OPTIONS} -fPIC)


if(WIN32)
    # Resource options
endif(WIN32)

# Library path
link_directories(
    .
)

# Define the CXX sources
set ( CXX_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_RTAS_1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_RTAS_2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_RTAS_3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_RTAS_4.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_RTAS_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_Unity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_processors.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_graphics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_core.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_gui_basics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_gui_extra.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mx_tune.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pitch_detector_aubio.cpp
    ${CMAKE_CURRENT_LIST_DIR}/auto_tune.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pitch_detector_talent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_data_structures.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pitch_shifter_talent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/Source/PluginProcessor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/Source/PluginGui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_AAX.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pitch_shifter_st.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/Source/KeyDetectGui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_basics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ring_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/Source/PluginParameter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp
    ${CMAKE_CURRENT_LIST_DIR}/manual_tune.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JUCE/JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -Os -O2 -std=c++14 -fpermissive -fPIC -Wextra -Weffc++ -Woverloaded-virtual -Wreorder -Wsign-promo -Wstrict-null-sentinel -Wfloat-equal -Wreturn-type -Wsign-compare -Wsizeof-pointer-memaccess -Woverflow -Wshadow -Wsequence-point -Wswitch-default -Wunused-but-set-parameter -Wunused-but-set-variable -Wunused-function -Wunused-label -Wunused-variable -Wmaybe-uninitialized -Wuninitialized -Wdisabled-optimization -Wdelete-non-virtual-dtor -Wno-non-virtual-dtor")

if(WIN32)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> ${RC_OPTIONS} -O coff -i <SOURCE> -o <OBJECT>")
endif(WIN32)



#{{{{ User Code 2
# Place your code here
IF (WIN32)
    set(LINK_OPTIONS ${LINK_OPTIONS} -static)
ELSEIF (UNIX)
    set(CMAKE_C_FLAGS "-I/usr/include/freetype2 -I/usr/include/libpng16")
    set(CMAKE_CXX_FLAGS "-I/usr/include/freetype2 -I/usr/include/libpng16")
    set(LINK_OPTIONS ${LINK_OPTIONS} "-lGL -lpthread -ldl  -s -lX11 -lXext -lfreetype -Wl,--no-undefined -Wl,-Bstatic -lSoundTouch -laubio -lfftw3f -Wl,-Bdynamic -fPIC")
ENDIF ()
#}}}}

add_library(mx_tune SHARED ${RC_SRCS} ${CXX_SRCS} ${C_SRCS})
target_link_libraries(mx_tune ${LINK_OPTIONS})



#{{{{ User Code 3
# Place your code here
IF (WIN32)
    target_link_libraries(mx_tune
        gdi32
        user32
        kernel32
        comctl32
        winmm
        uuid
        wsock32
        wininet
        version
        ole32
        ws2_32
        oleaut32
        imm32
        comdlg32
        shlwapi
        rpcrt4
        opengl32
        aubio
        fftw3f
        SoundTouch
    )

ELSEIF (APPLE)
ELSEIF (UNIX)
ENDIF ()
#}}}}

