cmake_minimum_required(VERSION 3.15)
project(overseer-plugin VERSION 1.0.0)

# CRITICAL: Enable Qt AUTOMOC for Q_OBJECT processing
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(libobs REQUIRED)
find_package(obs-frontend-api REQUIRED)

set(PLUGIN_NAME "overseer-plugin")

# Source files
set(SOURCES src/overseer-plugin.cpp)

# Headers for moc processing
set(HEADERS src/overseer-dock.h)

add_library(${PLUGIN_NAME} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${PLUGIN_NAME} PRIVATE
    ${OBS_INCLUDE_DIRS}
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    OBS::obs-frontend-api
    OBS::libobs
    Qt6::Core
    Qt6::Widgets
)

# Output to correct OBS plugin directory
set_target_properties(${PLUGIN_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/64bit"
)

# Install to OBS plugin directory
install(TARGETS ${PLUGIN_NAME}
    LIBRARY DESTINATION "${OBS_PLUGIN_DIR}/${PLUGIN_NAME}/bin/64bit"
)

 