# SpriteAI Editor Desktop

A cross-platform desktop sprite editor built with Qt6 and C++20.

## Requirements

### All Platforms
- **CMake** 3.20 or higher
- **Ninja** build system
- **Qt6** (6.8.0 or higher recommended)
  - Components: Core, Gui, Widgets, Network, Svg, LinguistTools
- **C++20** compatible compiler

### Platform-Specific

| Platform | Compiler | Qt Path (default) |
|----------|----------|-------------------|
| macOS | Clang (Xcode) | `/Users/<user>/Qt/6.x.x/macos` |
| Windows | MSVC 2022 | `C:/Qt/6.x.x/msvc2022_64` |
| Linux | GCC | `$HOME/Qt/6.x.x/gcc_64` |

### Installing Dependencies

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake and Ninja via Homebrew
brew install cmake ninja

# Install Qt6 via Qt Online Installer
# https://www.qt.io/download-qt-installer
```

**Windows:**
```powershell
# Install Visual Studio 2022 with C++ workload
# Install CMake: https://cmake.org/download/
# Install Ninja: https://ninja-build.org/
# Install Qt6 via Qt Online Installer
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install build-essential cmake ninja-build
sudo apt install qt6-base-dev qt6-tools-dev libqt6svg6-dev

# Or install Qt via Qt Online Installer for latest version
```

---

## Project Structure

```
spriteai-editor-desktop/
├── src/                    # Source files
│   ├── app/               # Application code
│   └── i18n/              # Internationalization
├── include/               # Header files
├── resources/             # Resources
│   ├── ui/               # Qt Designer .ui files
│   ├── i18n/             # Translation files
│   └── icons.qrc         # Icon resources
├── cmake-build-debug/     # Debug build output
├── cmake-build-release/   # Release build output
├── debug/                 # Debug app (for testing)
├── release/               # Standalone release app
├── CMakeLists.txt         # CMake configuration
└── CMakePresets.json      # Build presets
```

---

## Building

### Available Build Profiles

| Profile | Type | Output | Description |
|---------|------|--------|-------------|
| `debug` | Debug | `debug/` | Development build with debug symbols |
| `release` | Release | `release/` | Optimized standalone build |
| `relwithdebinfo` | Release | `release/` | Release with debug symbols |
| `minsizerel` | Release | `release/` | Size-optimized release |

### Command Line Build

```bash
# Configure (choose one profile)
cmake --preset debug
cmake --preset release
cmake --preset relwithdebinfo
cmake --preset minsizerel

# Build
cmake --build cmake-build-debug      # for debug
cmake --build cmake-build-release    # for release
```

### One-liner Build Commands

```bash
# Debug build
cmake --preset debug && cmake --build cmake-build-debug

# Release build (standalone)
cmake --preset release && cmake --build cmake-build-release
```

---

## IDE Setup

### CLion

1. Open the project folder in CLion
2. CLion will detect `CMakePresets.json` automatically
3. Go to **Settings → Build, Execution, Deployment → CMake**
4. Enable the profiles you want:
   - `debug`
   - `release`
   - `relwithdebinfo`
   - `minsizerel`
5. Select profile from the dropdown (top-right)
6. Click **Build** (hammer icon) or press `Cmd+F9` / `Ctrl+F9`

### Qt Creator

1. Open `CMakeLists.txt` in Qt Creator
2. Select the build presets when prompted
3. Configure and build as usual

### Visual Studio Code

1. Install extensions:
   - CMake Tools
   - C/C++
2. Open project folder
3. CMake Tools will detect presets
4. Select preset from status bar
5. Press `F7` to build

---

## Running the Application

### From Command Line

**macOS:**
```bash
# Debug (requires Qt installed)
open debug/SpriteAI.app

# Release (standalone - no Qt needed)
open "release/SpriteAI Editor.app"
```

**Windows:**
```cmd
# Debug
debug\SpriteAI.exe

# Release
release\SpriteAI.exe
```

**Linux:**
```bash
# Debug (requires Qt installed)
./debug/SpriteAI

# Release
./release/SpriteAI
```

### From IDE

- **CLion**: Select Run Configuration → Click Run (green play button)
- **Qt Creator**: Click Run or press `Ctrl+R`
- **VS Code**: Use CMake Tools "Run" button

---

## Debugging

### CLion

1. Select `debug` profile
2. Set breakpoints by clicking line numbers
3. Click **Debug** (bug icon) or press `Cmd+D` / `Ctrl+D`

### Qt Creator

1. Select Debug build
2. Set breakpoints
3. Press `F5` to start debugging

### VS Code

1. Select `debug` preset
2. Create `launch.json` for debugging:
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/debug/SpriteAI.app/Contents/MacOS/SpriteAI",
      "cwd": "${workspaceFolder}",
      "MIMode": "lldb"
    }
  ]
}
```
3. Press `F5` to debug

### Command Line (LLDB/GDB)

**macOS:**
```bash
lldb debug/SpriteAI.app/Contents/MacOS/SpriteAI
```

**Linux:**
```bash
gdb ./debug/SpriteAI
```

---

## Release Distribution

### macOS

The `release/SpriteAI Editor.app` is a standalone application bundle with all Qt frameworks included. Users can:
1. Copy the `.app` to `/Applications`
2. Double-click to run
3. No Qt installation required

**To create a DMG installer:**
```bash
hdiutil create -volname "SpriteAI Editor" -srcfolder release/ -ov -format UDZO SpriteAI-Editor.dmg
```

### Windows

The `release/` folder contains the `.exe` and all required DLLs. Users can:
1. Copy the entire `release/` folder
2. Run `SpriteAI.exe`
3. No Qt installation required

**To create an installer:** Use NSIS or Inno Setup with the `release/` folder contents.

### Linux

The `release/` folder contains the executable. For full standalone distribution:
1. Use `linuxdeployqt` to bundle Qt libraries
2. Or create an AppImage

---

## Custom Qt Path

If your Qt installation is in a different location, create `CMakeUserPresets.json`:

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
    },
    {
      "name": "release",
      "inherits": "release",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "/your/custom/Qt/path"
      }
    }
  ]
}
```

This file is gitignored and won't affect other developers.

---

## Technical Details

- **i18n**: Qt Linguist (.ts/.qm) + QTranslator
- **Input**: Qt events → engine ToolInput (screen→world coordinates)
- **Render**: CPU renderer buffer → QImage → QPainter
- **Engine**: Links to `spriteai-engine` (sibling directory)

---

## Troubleshooting

### "Qt6 not found"
- Ensure Qt6 is installed
- Check `CMAKE_PREFIX_PATH` points to correct Qt installation
- Create `CMakeUserPresets.json` with your Qt path

### "Ninja not found"
- Install Ninja: `brew install ninja` (macOS) or download from ninja-build.org

### macOS: "App is damaged" warning
```bash
xattr -cr "release/SpriteAI Editor.app"
```

### Windows: Missing DLLs
- Ensure you're running from the `release/` folder (not just the .exe)
- All required DLLs should be in the same folder as the .exe

### CLion: Only shows "Debug" profile
1. Delete `.idea` folder
2. Reopen project
3. Go to **Settings → Build, Execution, Deployment → CMake**
4. Click **+** to add profiles or enable existing ones

---

## License

MIT License - See LICENSE file for details.
