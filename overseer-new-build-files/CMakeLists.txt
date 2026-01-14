cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(overseer VERSION 1.0.0 LANGUAGES CXX)



set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_INCLUDE_CURRENT_DIR ON)



find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

find_package(libobs REQUIRED)

find_package(obs-frontend-api REQUIRED)



set(SOURCES

    overseer-plugin.cpp

    overseer-dock.cpp

    overseer-dock.h

    overseer-overlay.cpp

    overseer-overlay.h

    overseer-nested-scene.cpp

    overseer-nested-scene.h

    overseer-data.cpp

    overseer-data.h

    overseer-types.h

)



add_library(overseer MODULE ${SOURCES})



set_target_properties(overseer PROPERTIES

    AUTOMOC ON

    AUTOUIC ON

    AUTORCC ON

    CXX_STANDARD 17

    CXX_STANDARD_REQUIRED YES

)



target_include_directories(overseer PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}

    ${LIBOBS_INCLUDE_DIR}

    ${OBS_FRONTEND_API_INCLUDE_DIR}

)



target_link_libraries(overseer

    Qt5::Widgets

    Qt5::Core

    Qt5::Gui

    libobs

    obs-frontend-api

)



if(APPLE)

    install(TARGETS overseer LIBRARY DESTINATION "Library/Application Support/obs-studio/plugins/overseer/bin")

elseif(WIN32)

    install(TARGETS overseer LIBRARY DESTINATION "obs-studio/plugins/overseer/bin")

else()

    install(TARGETS overseer LIBRARY DESTINATION "lib/obs-plugins")

endif()
