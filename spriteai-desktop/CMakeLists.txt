cmake_minimum_required(VERSION 3.21)
project(SpriteAI_Desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Vulkan REQUIRED)

set(SPRITEAI_DESKTOP_SOURCES
    src/app/main.cpp
    src/app/MainWindow.cpp
    src/app/StatusBarController.cpp
    src/vulkan/VulkanEngine.cpp
)

set(SPRITEAI_DESKTOP_HEADERS
    src/app/MainWindow.h
    src/app/StatusBarController.h
    src/vulkan/VulkanEngine.h
)

add_executable(spriteai-desktop
    ${SPRITEAI_DESKTOP_SOURCES}
    ${SPRITEAI_DESKTOP_HEADERS}
)

target_include_directories(spriteai-desktop PRIVATE src)

target_link_libraries(spriteai-desktop
    Qt6::Widgets
    Vulkan::Vulkan
)

if(APPLE)
    set_target_properties(spriteai-desktop PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.spriteai.desktop"
        MACOSX_BUNDLE_BUNDLE_NAME "Sprite AI"
    )
endif()
