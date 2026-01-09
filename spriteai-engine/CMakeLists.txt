cmake_minimum_required(VERSION 3.20)

project(spriteai_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(spriteai_engine STATIC
        src/canvas/CanvasView.cpp

        src/core/command/AddStrokeCommand.cpp
        src/core/command/CommandStack.cpp
        src/core/document/SpriteDocument.cpp

        src/core/tools/ToolRegistry.cpp
        src/core/tools/PluginLoader.cpp
        src/core/tools/builtin/BrushTool.cpp
        src/core/tools/builtin/AITool.cpp
        src/core/tools/builtin/PencilTool.cpp
        src/core/tools/builtin/FillTool.cpp
        src/core/tools/builtin/EyedropperTool.cpp
        src/core/tools/builtin/SelectTool.cpp
        src/core/tools/builtin/MoveTool.cpp

        src/engine/EngineContext.cpp
        src/engine/renderer/cpu/CpuCanvasRenderer.cpp
        src/engine/renderer/vulkan/VulkanCanvasRenderer.cpp
)

target_include_directories(spriteai_engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(spriteai_engine
        PUBLIC
        Qt6::Core
)

target_compile_features(spriteai_engine PUBLIC cxx_std_20)
