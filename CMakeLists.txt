cmake_minimum_required(VERSION 3.21)

# set the project name
project(FileGrunt     VERSION 0.0.0
        DESCRIPTION "Utility to identify duplicate files/images and allow the user to move the duplicates to a location for backup or deletion.")

OPTION(sqlcipher "Build with SQLCipher library" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE)
elseif(WIN32)
    add_executable(${PROJECT_NAME} WIN32)
else()
    add_executable(${PROJECT_NAME})
endif()

set(CMAKE_PREFIX_PATH  "/mnt/ntfs1/Development/Qt/6.2.2/gcc_64/lib/cmake/")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(OpenCV_DIR "/mnt/ntfs1/Development/opencv-4.5.5/x64_build/")

set(CMAKE_CXX_STANDARD 14)
set(OpenCV_FOUND 1)
FIND_PACKAGE( OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# add the executable
#add_executable(FileGrunt src/main.cpp src/MainWindow.h src/MainWindow.cpp src/sqlite3.h src/sqlite3.c src/MainWindow.ui)

target_link_libraries(FileGrunt
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        sqlite3
        ${CMAKE_DL_LIBS}
        ${OpenCV_LIBS}
        ${LIBSQLITE}
        ${QT_LIBS}
        )

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

target_sources(${PROJECT_NAME}
        PRIVATE
        src/main.cpp
        src/MainWindow.h
        src/MainWindow.cpp
        src/MainWindow.ui
        src/sqlite3.h
        src/sqlite3.c
        )


