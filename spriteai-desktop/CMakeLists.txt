cmake_minimum_required(VERSION 3.21)

project(SpriteAI_Desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Vulkan
find_package(Vulkan REQUIRED)

# Sources
set(SPRITEAI_DESKTOP_SOURCES
        ${CMAKE_SOURCE_DIR}/src/app/main.cpp
        ${CMAKE_SOURCE_DIR}/src/app/MainWindow.cpp
        ${CMAKE_SOURCE_DIR}/src/app/StatusBarController.cpp
        ${CMAKE_SOURCE_DIR}/src/vulkan/VulkanEngine.cpp
)

# Headers
set(SPRITEAI_DESKTOP_HEADERS
        ${CMAKE_SOURCE_DIR}/src/app/MainWindow.h
        ${CMAKE_SOURCE_DIR}/src/app/StatusBarController.h
        ${CMAKE_SOURCE_DIR}/src/vulkan/VulkanEngine.h
)

add_executable(spriteai-desktop
        ${SPRITEAI_DESKTOP_SOURCES}
        ${SPRITEAI_DESKTOP_HEADERS}
)

target_include_directories(spriteai-desktop PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/app
        ${CMAKE_SOURCE_DIR}/src/vulkan
)

target_link_libraries(spriteai-desktop
        Qt6::Widgets
        Vulkan::Vulkan
)

# macOS Bundle
if(APPLE)
    set(MY_PLIST ${CMAKE_SOURCE_DIR}/Info.plist)

    set_target_properties(spriteai-desktop PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST ${MY_PLIST}
            MACOSX_BUNDLE_BUNDLE_NAME "Sprite AI"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.spriteai.desktop"
    )
endif()
