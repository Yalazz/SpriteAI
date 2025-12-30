cmake_minimum_required(VERSION 3.20)
project(SpriteAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(spriteai-engine)
add_subdirectory(spriteai-editor-desktop)
add_subdirectory(spriteai-plugins/brush_plus)
