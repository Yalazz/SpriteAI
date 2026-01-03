cmake_minimum_required(VERSION 3.20)
project(SpriteAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Auto-detect Qt installation if CMAKE_PREFIX_PATH is not set
if(NOT CMAKE_PREFIX_PATH)
    if(WIN32)
        # Try common Qt installation paths on Windows
        set(_QT_SEARCH_PATHS
            "C:/Qt/6.9.3/mingw_64"
            "C:/Qt/6.9.3/msvc2022_64"
            "C:/Qt/6.8.3/mingw_64"
            "C:/Qt/6.8.3/msvc2022_64"
            "$ENV{USERPROFILE}/Qt/6.9.3/mingw_64"
            "$ENV{USERPROFILE}/Qt/6.9.3/msvc2022_64"
        )
        foreach(_QT_PATH ${_QT_SEARCH_PATHS})
            if(EXISTS "${_QT_PATH}/lib/cmake/Qt6")
                set(CMAKE_PREFIX_PATH "${_QT_PATH}" CACHE PATH "Qt installation path")
                message(STATUS "Auto-detected Qt at: ${_QT_PATH}")
                break()
            endif()
        endforeach()
    elseif(APPLE)
        # macOS Qt path
        set(_QT_PATH "/Users/emrebilici/Qt/6.10.0/macos")
        if(EXISTS "${_QT_PATH}/lib/cmake/Qt6")
            set(CMAKE_PREFIX_PATH "${_QT_PATH}" CACHE PATH "Qt installation path")
            message(STATUS "Auto-detected Qt at: ${_QT_PATH}")
        endif()
    endif()
endif()

add_subdirectory(spriteai-engine)
add_subdirectory(spriteai-editor-desktop)
add_subdirectory(spriteai-plugins/brush_plus)
