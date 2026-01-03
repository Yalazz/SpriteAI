cmake_minimum_required(VERSION 3.20)
project(spriteai_editor_desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network LinguistTools)

set(I18N_TS_FILES
        resources/i18n/spriteai_tr.ts
        resources/i18n/spriteai_en.ts
)
qt6_add_translation(I18N_QM_FILES ${I18N_TS_FILES})

add_executable(spriteai_editor_desktop
        # ---- cpp ----
        src/app/main.cpp
        src/app/MainWindow.cpp
        src/app/CanvasWidget.cpp
        src/app/ConfigHotReload.cpp
        src/app/QtAIClient.cpp
        src/i18n/I18n.cpp

        include/spriteai/editor/app/MainWindow.h
        include/spriteai/editor/app/CanvasWidget.h
        include/spriteai/editor/app/ConfigHotReload.h
        include/spriteai/editor/app/QtAIClient.h

        include/spriteai/editor/i18n/I18n.h

        resources/i18n/i18n.qrc
        ${I18N_QM_FILES}
)

target_include_directories(spriteai_editor_desktop PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../spriteai-engine/include
)

# Add engine subdirectory only if building standalone (not from root)
if(NOT TARGET spriteai_engine)
    add_subdirectory(../spriteai-engine spriteai-engine-build)
endif()

target_link_libraries(spriteai_editor_desktop PRIVATE
        spriteai_engine
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)

if(APPLE)
    set_target_properties(spriteai_editor_desktop PROPERTIES MACOSX_BUNDLE TRUE)
endif()
