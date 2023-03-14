###################################################################################
################## Butianyun QT Video Lesson V2 ###################################
########### BUTIANYUN， QT Programming Trainning Professional ######################
####################################################################################

cmake_minimum_required(VERSION 3.5)

project(ButianyunQMLCacheLoader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS  Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick)

set(PROJECT_SOURCES
        butianyun_qml_cache_loader.cpp
)


add_library(ButianyunQMLCacheLoader SHARED
    ${PROJECT_SOURCES}
)


target_link_libraries(ButianyunQMLCacheLoader PRIVATE Qt${QT_VERSION_MAJOR}::Quick)

set_target_properties(ButianyunQMLCacheLoader PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER butianyun.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ButianyunQMLCacheLoader
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ButianyunQMLCacheLoader)
endif()

