cmake_minimum_required(VERSION 3.20)
project(spriteai_plugin_brush_plus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(spriteai_plugin_brush_plus SHARED
        BrushPlusPlugin.cpp
)

target_include_directories(spriteai_plugin_brush_plus
        PRIVATE
        ${CMAKE_SOURCE_DIR}/spriteai-engine/include
)

target_link_libraries(spriteai_plugin_brush_plus
        PRIVATE
        spriteai_engine
        Qt6::Core
)

# Set output directories for plugins
# LIBRARY_OUTPUT_DIRECTORY is for .so/.dylib (Unix)
# RUNTIME_OUTPUT_DIRECTORY is for .dll (Windows)
set_target_properties(spriteai_plugin_brush_plus PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
