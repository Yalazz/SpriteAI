cmake_minimum_required(VERSION 3.20)
project(spriteai_editor_desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set UI output directory
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/resources/ui)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network LinguistTools Svg)

# i18n translation files
set(I18N_TS_FILES
        resources/i18n/spriteai_tr.ts
        resources/i18n/spriteai_en.ts
)
qt6_add_translation(I18N_QM_FILES ${I18N_TS_FILES})

# UI files
set(UI_FILES
        resources/ui/MainWindow.ui
        resources/ui/ToolsDock.ui
        resources/ui/ColorDock.ui
        resources/ui/BrushSettingsDock.ui
        resources/ui/LayersDock.ui
        resources/ui/HistoryDock.ui
)

# Source files
set(SOURCES
        src/app/main.cpp
        src/app/MainWindow.cpp
        src/app/CanvasWidget.cpp
        src/app/ConfigHotReload.cpp
        src/app/QtAIClient.cpp
        src/app/ToolsDockWidget.cpp
        src/app/ColorDockWidget.cpp
        src/app/BrushSettingsDockWidget.cpp
        src/app/LayersDockWidget.cpp
        src/app/HistoryDockWidget.cpp
        src/i18n/I18n.cpp
)

# Header files
set(HEADERS
        include/spriteai/editor/app/MainWindow.h
        include/spriteai/editor/app/CanvasWidget.h
        include/spriteai/editor/app/ConfigHotReload.h
        include/spriteai/editor/app/QtAIClient.h
        include/spriteai/editor/app/ToolsDockWidget.h
        include/spriteai/editor/app/ColorDockWidget.h
        include/spriteai/editor/app/BrushSettingsDockWidget.h
        include/spriteai/editor/app/LayersDockWidget.h
        include/spriteai/editor/app/HistoryDockWidget.h
        include/spriteai/editor/i18n/I18n.h
)

# Resource files
set(RESOURCES
        resources/i18n/i18n.qrc
        resources/icons.qrc
)

add_executable(spriteai_editor_desktop
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${RESOURCES}
        ${I18N_QM_FILES}
)

target_include_directories(spriteai_editor_desktop PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../spriteai-engine/include
        ${CMAKE_CURRENT_BINARY_DIR}  # For ui_*.h files
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
        Qt6::Svg
)

# Platform-specific configurations
if(APPLE)
    set_target_properties(spriteai_editor_desktop PROPERTIES MACOSX_BUNDLE TRUE)
elseif(WIN32)
    # WIN32_EXECUTABLE makes it a GUI application (no console window)
    set_target_properties(spriteai_editor_desktop PROPERTIES WIN32_EXECUTABLE TRUE)

    # Get Qt installation directory
    get_target_property(_qt_core_location Qt6::Core LOCATION)
    get_filename_component(_qt_bin_dir "${_qt_core_location}" DIRECTORY)
    get_filename_component(_qt_install_dir "${_qt_bin_dir}" DIRECTORY)

    # Copy Qt DLLs to output directory for easier debugging/running
    add_custom_command(TARGET spriteai_editor_desktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt6::Core>
            $<TARGET_FILE:Qt6::Gui>
            $<TARGET_FILE:Qt6::Widgets>
            $<TARGET_FILE:Qt6::Network>
            $<TARGET_FILE_DIR:spriteai_editor_desktop>
    )

    # Copy Qt platform plugins (required for Qt to run on Windows)
    add_custom_command(TARGET spriteai_editor_desktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:spriteai_editor_desktop>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_qt_install_dir}/plugins/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:spriteai_editor_desktop>/platforms/"
    )

    # Copy Qt styles plugins (for native look)
    add_custom_command(TARGET spriteai_editor_desktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:spriteai_editor_desktop>/styles"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${_qt_install_dir}/plugins/styles"
            "$<TARGET_FILE_DIR:spriteai_editor_desktop>/styles/"
    )

    # Copy Qt icon engine plugins (for SVG icons)
    add_custom_command(TARGET spriteai_editor_desktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:spriteai_editor_desktop>/iconengines"
    )

    # Copy SVG support DLLs if available
    if(EXISTS "${_qt_install_dir}/plugins/iconengines")
        add_custom_command(TARGET spriteai_editor_desktop POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${_qt_install_dir}/plugins/iconengines"
                "$<TARGET_FILE_DIR:spriteai_editor_desktop>/iconengines/"
        )
    endif()

    # Copy Qt image format plugins
    add_custom_command(TARGET spriteai_editor_desktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:spriteai_editor_desktop>/imageformats"
    )
    if(EXISTS "${_qt_install_dir}/plugins/imageformats")
        add_custom_command(TARGET spriteai_editor_desktop POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${_qt_install_dir}/plugins/imageformats"
                "$<TARGET_FILE_DIR:spriteai_editor_desktop>/imageformats/"
        )
    endif()
endif()
