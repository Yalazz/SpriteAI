# spriteai-engine

**Amaç:** Editor/UI’dan tamamen bağımsız bir çekirdek ve render altyapısı.

- Document = single source of truth
- Tool + Command (Undo/Redo) Document’i değiştirir
- Renderer sadece render eder (CPU/Vulkan backend)
- Engine, platformdan bağımsız C++20 (Qt bağımlılığı yok)

Editor bu repoyu submodule gibi ekleyebilir.
