# Sprite AI

A professional sprite editor with AI-powered features, built with a modular architecture inspired by Unity/Unreal Engine.

## Overview

Sprite AI is designed with a **separation of concerns** approach where the core engine is completely independent from the editor UI. This allows for:

- Cross-platform support (macOS, Windows, Linux)
- Multiple frontend implementations
- Easy testing and maintenance
- Plugin extensibility

## Project Structure

```
SpriteAI/
├── spriteai-engine/          # Core engine (C++20, no UI dependencies)
├── spriteai-editor-desktop/  # Qt6 Desktop Editor
├── spriteai-services/        # Backend API + AI microservices
├── spriteai-infra/           # Docker infrastructure (PostgreSQL, RabbitMQ, etc.)
├── spriteai-plugins/         # Plugin extensions
├── config/                   # Shared configuration (themes, tools, workspace)
├── CMakeLists.txt            # Root CMake configuration
├── CMakePresets.json         # Build presets
└── Jenkinsfile               # CI/CD pipeline
```

| Directory | Description |
|-----------|-------------|
| `spriteai-engine/` | Platform-independent core: Document model, Tools, Commands (Undo/Redo), Renderer API |
| `spriteai-editor-desktop/` | Qt6-based desktop application with docking, i18n, and input handling |
| `spriteai-services/` | Spring Boot backend + AI microservices for cloud features |
| `spriteai-infra/` | Docker Compose setup for local development infrastructure |
| `spriteai-plugins/` | Optional plugin modules (e.g., custom brushes) |
| `config/` | Themes, tool configurations, and workspace templates |

## Quick Start

### Prerequisites

- **CMake** 3.20+
- **Ninja** build system
- **Qt6** 6.8.0+ (for desktop editor)
- **C++20** compatible compiler
- **Docker** (optional, for backend services)

### Building the Desktop Editor

```bash
# Clone the repository
git clone https://github.com/user/SpriteAI.git
cd SpriteAI

# Configure and build (debug)
cmake --preset debug
cmake --build cmake-build-debug

# Or release build
cmake --preset release
cmake --build cmake-build-release
```

### Running the Application

**macOS:**
```bash
open spriteai-editor-desktop/debug/SpriteAI.app
```

**Windows:**
```cmd
spriteai-editor-desktop\debug\SpriteAI.exe
```

**Linux:**
```bash
./spriteai-editor-desktop/debug/SpriteAI
```

### Starting Backend Services (Optional)

```bash
cd spriteai-infra
docker-compose up -d
```

## Qt Installation

The project uses environment variables for Qt paths. Install Qt6 via the [Qt Online Installer](https://www.qt.io/download-qt-installer) to:

| Platform | Default Path |
|----------|--------------|
| macOS | `~/Qt/6.x.x/macos` |
| Windows | `%USERPROFILE%/Qt/6.x.x/msvc2022_64` |
| Linux | `~/Qt/6.x.x/gcc_64` |

If your Qt is installed elsewhere, create a `CMakeUserPresets.json` file (gitignored) to override:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "debug",
      "inherits": "debug",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "/your/custom/Qt/path"
      }
    }
  ]
}
```

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Desktop Editor (Qt6)                  │
│         UI, Docking, Input Handling, i18n               │
└─────────────────────────┬───────────────────────────────┘
                          │
┌─────────────────────────▼───────────────────────────────┐
│                    Engine Core (C++20)                   │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌───────────┐  │
│  │Document │  │  Tools  │  │Commands │  │ Renderer  │  │
│  │ (Data)  │  │ (Input) │  │(Undo/Do)│  │(CPU/Vulkan│  │
│  └─────────┘  └─────────┘  └─────────┘  └───────────┘  │
└─────────────────────────────────────────────────────────┘
                          │
┌─────────────────────────▼───────────────────────────────┐
│                   Backend Services                       │
│            Spring Boot API + AI Microservices           │
└─────────────────────────────────────────────────────────┘
```

## Documentation

Each component has its own README with detailed documentation:

- [Desktop Editor Documentation](spriteai-editor-desktop/README.md) - Build instructions, IDE setup, debugging
- [Engine Documentation](spriteai-engine/README.md) - Core architecture, API reference
- [Services Documentation](spriteai-services/README.md) - Backend API, AI services
- [Infrastructure Documentation](spriteai-infra/README.md) - Docker setup, ports, configuration

## Development

### IDE Setup

The project works with:
- **CLion** - Detects CMakePresets.json automatically
- **Qt Creator** - Open CMakeLists.txt
- **VS Code** - Use CMake Tools extension

### Build Profiles

| Profile | Description |
|---------|-------------|
| `debug` | Development build with debug symbols |
| `release` | Optimized standalone build |
| `relwithdebinfo` | Release with debug symbols |
| `minsizerel` | Size-optimized release |

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

MIT License - See [LICENSE](LICENSE) file for details.
