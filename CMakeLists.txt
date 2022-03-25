cmake_minimum_required(VERSION 3.16)
project(filegrunt     VERSION 0.0.0
        DESCRIPTION "Utility to identify duplicate files/images and allow the user to move the duplicates to a location for backup or deletion.")

#OPTION(BUILD_STABLE_VERSION "Don't build the stable version by default" OFF) # Choose between building a stable version or nightly (the default), depending on whether '-DBUILD_STABLE_VERSION=1' is passed on the command line or not.

OPTION(sqlcipher "Build with SQLCipher library" OFF)
# BUILD_VERSION is the current date in YYYYMMDD format. It is only
# used by the nightly version to add the date of the build.
#string(TIMESTAMP BUILD_VERSION "%Y%m%d")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE)
elseif(WIN32)
    add_executable(${PROJECT_NAME} WIN32)
else()
    add_executable(${PROJECT_NAME})
    #add_executable(filegrunt src/main.cpp src/MainWindow.h src/MainWindow.cpp src/sqlite3.h src/sqlite3.c src/MainWindow.ui)
    #add_executable(${PROJECT_NAME} src/main.cpp src/MainWindow.h src/MainWindow.cpp src/sqlite3.h src/sqlite3.c src/MainWindow.ui)
    #add_executable(${PROJECT_NAME} src/main.cpp src/MainWindow.cpp src/sqlite3.c src/MainWindow.ui)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")
set(CMAKE_PREFIX_PATH  "/mnt/ntfs1/Development/Qt/5.15.2/gcc_64/lib/cmake/")
#set(QT_VERSION 5)

#set(OpenCV_DIR "/mnt/ntfs1/Development/opencv-4.5.5/x64_build/")
set(OpenCV_DIR "/mnt/ntfs1/Development/opencv-4.5.5/")
set(OpenCV_FOUND 1)

find_package(OpenCV REQUIRED core imgproc imgcodecs)
#find_package(PkgConfig REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Qt5 REQUIRED COMPONENTS Gui Widgets)

target_sources(${PROJECT_NAME}
        PRIVATE
        src/main.cpp
        src/MainWindow.h
        src/MainWindow.cpp
        src/MainWindow.ui
        src/sqlite3.h
        src/sqlite3.c
        )

#set(QT_LIBS Qt5::Gui Qt5::Widgets Qt5::Core)
set(QT_LIBS Qt5::Gui Qt5::Widgets)

# https://stackoverflow.com/questions/34143265/undefined-reference-to-symbol-pthread-createglibc-2-2-5
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(LPTHREAD pthread)

target_link_libraries(${PROJECT_NAME}
        ${LPTHREAD}
        ${QT_LIBS}
        ${LIBSQLITE}
        #sqlite3
        ${CMAKE_DL_LIBS}
        ${OpenCV_LIBS}
        )

if(UNIX)
    install(FILES images/icon.png
            DESTINATION share/icons/hicolor/scalable/apps/
            RENAME ${PROJECT_NAME}.png
            )

    install(FILES distri/${PROJECT_NAME}.desktop
            DESTINATION share/applications/
            )

    install(FILES distri/${PROJECT_NAME}.desktop.appdata.xml
            DESTINATION share/metainfo/
            )

    #install(FILES data/offline.db
    #        DESTINATION ${HOME}/data/
    #        )
endif()

if(NOT WIN32 AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            )
endif()

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")