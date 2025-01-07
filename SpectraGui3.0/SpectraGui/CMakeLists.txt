
cmake_minimum_required(VERSION 3.5)

if (WIN32)
    project(SpectraGUI LANGUAGES CXX)
elseif(UNIX)
    project(SpectraGUI LANGUAGES CXX)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()
find_package(Qt5Charts)
find_package(QT NAMES Qt5 COMPONENTS Widgets Gui Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets  Concurrent REQUIRED)
find_package(gram_savitzky_golay REQUIRED)

SET(PROJECT_SOURCES)
SET(SOURCE_FILES)
SET(HEADER_FILES)
SET(IMPORTANT_FILES)


SET( HEADER_FILES
        headers/SpectraGui.h
        headers/mainwindow.h
        headers/as7265x.h
        headers/chart.h
        headers/chartview.h
        headers/predictions.h
        headers/savitzky_golay_filter.h


)
SET( SOURCE_FILES
        sources/SpectraGui.cpp
        sources/mainwindow.cpp
        sources/as7265x.cpp
        sources/chartview.cpp
        sources/chart.cpp
        sources/predictions.cpp
        sources/savitzky_golay_filter.cpp

)

set(IMPORTANT_FILES
        importantFiles/rc.qrc
        importantFiles/compilationType.h
)

LIST(APPEND PROJECT_SOURCES ${SOURCE_FILES})
LIST(APPEND PROJECT_SOURCES ${HEADER_FILES})
LIST(APPEND PROJECT_SOURCES ${IMPORTANT_FILES})
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Spe ${PROJECT_SOURCES})

else()

    if(WIN32)
        set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/rc.rc")
        add_executable(SpectraGUI WIN32 ${PROJECT_SOURCES} )
    elseif(UNIX)

        add_executable(SpectraGUI ${PROJECT_SOURCES})

    endif()

endif()

target_link_libraries(SpectraGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets  Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Concurrent gram_savitzky_golay::gram_savitzky_golay)
