cmake_minimum_required(VERSION 3.15)
project(overseer-plugin VERSION 1.0.0)
# Enable Qt AUTOMOC for Q_OBJECT processing
set(CMAKE_AUTOMOC ON)



# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# Add OBS include path
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include/obs")


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Plugin properties
set(PLUGIN_NAME "overseer-plugin")
set(PLUGIN_VERSION "1.0.0")
set(PLUGIN_DESCRIPTION "Overseer Plugin for OBS - Modular Overlay Control")
set(PLUGIN_AUTHOR "Pombomb Media")

# Source files
set(SOURCES
    src/overseer-plugin.cpp
    src/overseer-dock.h
)

# Create plugin library
add_library(${PLUGIN_NAME} SHARED ${SOURCES})

# Include directories
target_include_directories(${PLUGIN_NAME} PRIVATE
    ${OBS_INCLUDE_DIRS}
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PLUGIN_NAME} PRIVATE
    /usr/local/lib/libobs-frontend-api.so
    /usr/local/lib/libobs.so
    Qt6::Core
    Qt6::Widgets
)

# Set output directory
set_target_properties(${PLUGIN_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/64bit"
)

# Install plugin
install(TARGETS ${PLUGIN_NAME}
    LIBRARY DESTINATION "${OBS_PLUGIN_DIR}/${PLUGIN_NAME}/bin/64bit"
)

# Install data files
install(DIRECTORY data/
    DESTINATION "${OBS_PLUGIN_DIR}/${PLUGIN_NAME}/data"
    FILES_MATCHING PATTERN "*"
)

# Install locale files
install(DIRECTORY data/locale/
    DESTINATION "${OBS_PLUGIN_DIR}/${PLUGIN_NAME}/data/locale"
    FILES_MATCHING PATTERN "*.ini"
)
