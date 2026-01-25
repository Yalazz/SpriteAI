cmake_minimum_required(VERSION 3.20)
project(SpriteAI LANGUAGES CXX)

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

add_executable(SpriteAI
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${RESOURCES}
        ${I18N_QM_FILES}
)

target_include_directories(SpriteAI PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../spriteai-engine/include
        ${CMAKE_CURRENT_BINARY_DIR}  # For ui_*.h files
)

# Add engine subdirectory only if building standalone (not from root)
if(NOT TARGET spriteai_engine)
    add_subdirectory(../spriteai-engine spriteai-engine-build)
endif()

target_link_libraries(SpriteAI PRIVATE
        spriteai_engine
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Svg
)

# ============================================================================
# Determine build type (release vs debug)
# ============================================================================
set(IS_RELEASE_BUILD FALSE)
set(IS_DEBUG_BUILD FALSE)

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR
   CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR
   CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(IS_RELEASE_BUILD TRUE)
    set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(IS_DEBUG_BUILD TRUE)
    set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/debug")
endif()

# ============================================================================
# Platform-specific configurations
# ============================================================================
if(APPLE)
    set_target_properties(SpriteAI PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.spriteai.editor"
        MACOSX_BUNDLE_BUNDLE_NAME "SpriteAI Editor"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    )

    # Release: Deploy Qt frameworks and copy to release folder
    if(IS_RELEASE_BUILD)
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")

        if(MACDEPLOYQT_EXECUTABLE)
            add_custom_command(TARGET SpriteAI POST_BUILD
                COMMAND "${MACDEPLOYQT_EXECUTABLE}"
                    "$<TARGET_BUNDLE_DIR:SpriteAI>"
                    -always-overwrite
                COMMENT "Deploying Qt frameworks into app bundle..."
            )

            add_custom_command(TARGET SpriteAI POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}"
                COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_DIR}/SpriteAI Editor.app"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "$<TARGET_BUNDLE_DIR:SpriteAI>"
                    "${OUTPUT_DIR}/SpriteAI Editor.app"
                COMMENT "Copying standalone app to release/..."
            )
        else()
            message(WARNING "macdeployqt not found. Release build will not be standalone.")
        endif()
    endif()

    # Debug: Copy to debug folder (no Qt bundling)
    if(IS_DEBUG_BUILD)
        add_custom_command(TARGET SpriteAI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}"
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_DIR}/SpriteAI.app"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "$<TARGET_BUNDLE_DIR:SpriteAI>"
                "${OUTPUT_DIR}/SpriteAI.app"
            COMMENT "Copying app to debug/..."
        )
    endif()

elseif(WIN32)
    # WIN32_EXECUTABLE makes it a GUI application (no console window)
    set_target_properties(SpriteAI PROPERTIES WIN32_EXECUTABLE TRUE)

    # Get Qt installation directory
    get_target_property(_qt_core_location Qt6::Core LOCATION)
    get_filename_component(_qt_bin_dir "${_qt_core_location}" DIRECTORY)
    get_filename_component(_qt_install_dir "${_qt_bin_dir}" DIRECTORY)

    # Copy Qt DLLs to output directory for easier debugging/running
    add_custom_command(TARGET SpriteAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt6::Core>
            $<TARGET_FILE:Qt6::Gui>
            $<TARGET_FILE:Qt6::Widgets>
            $<TARGET_FILE:Qt6::Network>
            $<TARGET_FILE_DIR:SpriteAI>
    )

    # Copy Qt platform plugins (required for Qt to run on Windows)
    add_custom_command(TARGET SpriteAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:SpriteAI>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_qt_install_dir}/plugins/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:SpriteAI>/platforms/"
    )

    # Copy Qt styles plugins (for native look)
    add_custom_command(TARGET SpriteAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:SpriteAI>/styles"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${_qt_install_dir}/plugins/styles"
            "$<TARGET_FILE_DIR:SpriteAI>/styles/"
    )

    # Copy Qt icon engine plugins (for SVG icons)
    add_custom_command(TARGET SpriteAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:SpriteAI>/iconengines"
    )

    # Copy SVG support DLLs if available
    if(EXISTS "${_qt_install_dir}/plugins/iconengines")
        add_custom_command(TARGET SpriteAI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${_qt_install_dir}/plugins/iconengines"
                "$<TARGET_FILE_DIR:SpriteAI>/iconengines/"
        )
    endif()

    # Copy Qt image format plugins
    add_custom_command(TARGET SpriteAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:SpriteAI>/imageformats"
    )
    if(EXISTS "${_qt_install_dir}/plugins/imageformats")
        add_custom_command(TARGET SpriteAI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${_qt_install_dir}/plugins/imageformats"
                "$<TARGET_FILE_DIR:SpriteAI>/imageformats/"
        )
    endif()

    # Copy entire build to release/debug folder
    if(IS_RELEASE_BUILD OR IS_DEBUG_BUILD)
        add_custom_command(TARGET SpriteAI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "$<TARGET_FILE_DIR:SpriteAI>"
                "${OUTPUT_DIR}/"
            COMMENT "Copying build to ${OUTPUT_DIR}/..."
        )
    endif()
endif()
