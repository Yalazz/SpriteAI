cmake_minimum_required(VERSION 3.21)

project(spriteai_desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# -------------------------------------------------------------
# QT6
# -------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

# -------------------------------------------------------------
# VULKAN SDK
# -------------------------------------------------------------
find_package(Vulkan REQUIRED)

qt_standard_project_setup()

# -------------------------------------------------------------
# Kaynak Dosyaları
# -------------------------------------------------------------
set(SPRITEAI_SOURCES

        # Entry + MainWindow
        src/app/.cpp/main.cpp
        src/app/.cpp/MainWindow.cpp
        src/app/.h/MainWindow.h

        # Infinite Canvas (CPU)
        src/app/.cpp/CanvasWidget.cpp
        src/app/.h/CanvasWidget.h
        src/canvas/.cpp/CpuCanvasRenderer.cpp
        src/canvas/.h/CpuCanvasRenderer.h

        # (ICanvasRenderer şu an kullanılmıyor, istersen silebilirsin)
        # src/canvas/ICanvasRenderer.h

        # Engine
        src/engine/.cpp/EngineContext.cpp
        src/engine/.h/EngineContext.h
        src/engine/.cpp/Renderer.cpp
        src/engine/.h/Renderer.h

        # Vulkan (Temel – henüz kullanılmıyor)
        src/vulkan/.cpp/VulkanContext.cpp
        src/vulkan/.h/VulkanContext.h
        src/vulkan/.cpp/VulkanDevice.cpp
        src/vulkan/.h/VulkanDevice.h
        src/vulkan/.cpp/VulkanSurface.cpp
        src/vulkan/.h/VulkanSurface.h
        src/vulkan/.cpp/VulkanSwapchain.cpp
        src/vulkan/.h/VulkanSwapchain.h
        src/vulkan/.cpp/VulkanRenderPass.cpp
        src/vulkan/.h/VulkanRenderPass.h
        src/vulkan/.cpp/VulkanCommandPool.cpp
        src/vulkan/.h/VulkanCommandPool.h
        src/canvas/.cpp/CanvasLayer.cpp
        src/canvas/.h/CanvasLayer.h
        src/canvas/.cpp/CanvasManager.cpp
        src/canvas/.h/CanvasManager.h
        src/canvas/.cpp/InfiniteGridRenderer.cpp
        src/canvas/.h/InfiniteGridRenderer.h
        src/canvas/.cpp/CanvasState.cpp
        src/canvas/.h/CanvasState.h

        # ⚠ GPU Canvas dosyaları şimdilik devre Dışı!
        # src/canvas/VulkanCanvasRenderer.cpp
        # src/canvas/VulkanCanvasRenderer.h
        # src/vulkan/VulkanCanvasPipeline.cpp
        # src/vulkan/VulkanCanvasPipeline.h
        # src/vulkan/VulkanCanvasFramebuffer.cpp
        # src/vulkan/VulkanCanvasFramebuffer.h
)

# -------------------------------------------------------------
# EXECUTABLE
# -------------------------------------------------------------
add_executable(spriteai-desktop ${SPRITEAI_SOURCES})

target_include_directories(spriteai-desktop PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app
        ${CMAKE_CURRENT_SOURCE_DIR}/src/canvas
        ${CMAKE_CURRENT_SOURCE_DIR}/src/engine
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan
)

target_link_libraries(spriteai-desktop PRIVATE
        Qt6::Widgets
        Qt6::Gui
        Vulkan::Vulkan
)

# -------------------------------------------------------------
# macOS Frameworks
# -------------------------------------------------------------
if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(METAL_FRAMEWORK Metal)
    find_library(QUARTZ_CORE_FRAMEWORK QuartzCore)

    target_link_libraries(spriteai-desktop PRIVATE
            ${COCOA_FRAMEWORK}
            ${METAL_FRAMEWORK}
            ${QUARTZ_CORE_FRAMEWORK}
    )

    set_target_properties(spriteai-desktop PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME "Sprite AI Desktop"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.spriteai.desktop"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
    )
endif()
