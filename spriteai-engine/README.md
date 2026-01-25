# Sprite AI Engine

The core engine library for Sprite AI - completely independent from any UI framework.

## Overview

This is a **platform-independent C++20 library** that provides:

- Document model (single source of truth for sprite data)
- Tool system (Brush, Eraser, Selection, etc.)
- Command pattern (Undo/Redo support)
- Renderer API with CPU and Vulkan backends

## Architecture

```
┌─────────────────────────────────────────────────────┐
│                     Document                         │
│  (Layers, Frames, Pixels - Single Source of Truth)  │
└───────────────────────┬─────────────────────────────┘
                        │
        ┌───────────────┼───────────────┐
        │               │               │
        ▼               ▼               ▼
┌───────────┐   ┌───────────┐   ┌───────────┐
│   Tools   │   │ Commands  │   │ Renderer  │
│(Brush,etc)│   │(Undo/Redo)│   │(CPU/Vulkan│
└───────────┘   └───────────┘   └───────────┘
```

## Key Concepts

### Document
The Document is the single source of truth containing all sprite data:
- Layers (with blend modes, opacity)
- Frames (for animation)
- Pixel data (RGBA)

### Tools
Tools handle user input and generate Commands:
- Brush, Pencil, Eraser
- Selection tools (Rectangle, Lasso, Magic Wand)
- Fill, Gradient
- Shape tools

### Commands
All modifications to the Document go through Commands, enabling:
- Full Undo/Redo support
- Action history
- Macro recording

### Renderer
Abstract renderer API with multiple backends:
- **CPU Renderer**: Software rendering, always available
- **Vulkan Renderer**: GPU-accelerated (optional)

## Building

The engine is built as part of the main project:

```bash
# From repository root
cmake --preset debug
cmake --build cmake-build-debug
```

Or standalone:

```bash
cd spriteai-engine
mkdir build && cd build
cmake ..
cmake --build .
```

## Integration

The editor integrates the engine via CMake `add_subdirectory`:

```cmake
add_subdirectory(../spriteai-engine ${CMAKE_BINARY_DIR}/spriteai-engine)
target_link_libraries(YourApp PRIVATE spriteai-engine)
```

## Dependencies

- C++20 compiler
- No Qt dependency
- No platform-specific code in core

## API Example

```cpp
#include <spriteai/Document.h>
#include <spriteai/tools/BrushTool.h>
#include <spriteai/commands/CommandStack.h>

// Create document
auto doc = std::make_shared<Document>(64, 64);

// Create tool
BrushTool brush;
brush.setSize(5);
brush.setColor({255, 0, 0, 255});

// Handle input (generates command)
auto cmd = brush.onMouseDown(doc, {32, 32});
commandStack.execute(std::move(cmd));

// Undo
commandStack.undo();
```

## License

MIT License - See [LICENSE](../LICENSE) file for details.
