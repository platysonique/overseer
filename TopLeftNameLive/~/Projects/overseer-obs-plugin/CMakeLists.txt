cmake_minimum_required(VERSION 3.15)
project(overseer-plugin VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt AUTOMOC for Q_OBJECT processing
set(CMAKE_AUTOMOC ON)

# Add OBS include path and export symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include/obs -fvisibility=default")

# Plugin properties
set(PLUGIN_NAME "overseer-plugin")
set(PLUGIN_VERSION "1.0.0")
set(PLUGIN_DESCRIPTION "Overseer Plugin for OBS - Modular Overlay Control")
set(PLUGIN_AUTHOR "Pombomb Media")

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Source files
set(SOURCES
    src/overseer-plugin.cpp
    src/overseer-dock.h
)

# Create plugin library
add_library(${PLUGIN_NAME} SHARED ${SOURCES})

# Include directories
include_directories(/usr/local/include/obs)
target_include_directories(${PLUGIN_NAME} PRIVATE
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PLUGIN_NAME} PRIVATE
    /usr/local/lib/libobs-frontend-api.so
    /usr/local/lib/libobs.so
    Qt6::Widgets
)

# Set output directory
set_target_properties(${PLUGIN_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/64bit"
)
