cmake_minimum_required(VERSION 3.21)
project(spriteai_desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui LinguistTools)
find_package(Vulkan REQUIRED)

# -------------------------------------------------------------
# Sources
# -------------------------------------------------------------
set(SPRITEAI_SOURCES
        src/app/main.cpp
        src/app/MainWindow.cpp
        include/app/MainWindow.h
        src/app/CanvasWidget.cpp
        include/app/CanvasWidget.h

        src/core/DrawingModel.cpp
        include/core/DrawingModel.h

        # ---- Canvas core ----
        src/canvas/CanvasSession.cpp
        include/canvas/CanvasSession.h
        include/canvas/CanvasWorkflow.h
        include/canvas/CanvasView.h
        include/canvas/CanvasLayer.h
        include/canvas/workflows/InfiniteGridWorkflow.h
        include/canvas/workflows/BoundedWorkflow.h

        # ---- Renderers ----
        src/canvas/renderer/CpuCanvasRenderer.cpp
        include/canvas/renderer/CpuCanvasRenderer.h
        src/canvas/renderer/InfiniteGridRenderer.cpp
        include/canvas/renderer/InfiniteGridRenderer.h

        # ---- Vulkan ----
        src/vulkan/VulkanContext.cpp
        include/vulkan/VulkanContext.h

        # ---- i18n ----
        resources/i18n/i18n.qrc
)

add_executable(spriteai-desktop ${SPRITEAI_SOURCES})

target_include_directories(spriteai-desktop PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(spriteai-desktop PRIVATE
        Qt6::Widgets
        Qt6::Gui
        Vulkan::Vulkan
)

# -------------------------------------------------------------
# i18n (HER QM İÇİN AYRI RULE — ŞART)
# -------------------------------------------------------------
set(I18N_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/i18n)

add_custom_command(
        OUTPUT ${I18N_DIR}/spriteai_tr.qm
        COMMAND Qt6::lrelease ${I18N_DIR}/spriteai_tr.ts -qm ${I18N_DIR}/spriteai_tr.qm
        DEPENDS ${I18N_DIR}/spriteai_tr.ts
        COMMENT "Generating spriteai_tr.qm"
)

add_custom_command(
        OUTPUT ${I18N_DIR}/spriteai_en.qm
        COMMAND Qt6::lrelease ${I18N_DIR}/spriteai_en.ts -qm ${I18N_DIR}/spriteai_en.qm
        DEPENDS ${I18N_DIR}/spriteai_en.ts
        COMMENT "Generating spriteai_en.qm"
)

add_custom_target(spriteai_translations ALL
        DEPENDS
        ${I18N_DIR}/spriteai_tr.qm
        ${I18N_DIR}/spriteai_en.qm
)

add_dependencies(spriteai-desktop spriteai_translations)

# -------------------------------------------------------------
# macOS
# -------------------------------------------------------------
if(APPLE)
    find_library(COCOA Cocoa)
    find_library(METAL Metal)
    find_library(QUARTZ QuartzCore)

    target_link_libraries(spriteai-desktop PRIVATE
            ${COCOA}
            ${METAL}
            ${QUARTZ}
    )

    set_target_properties(spriteai-desktop PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.spriteai.desktop"
    )
endif()
