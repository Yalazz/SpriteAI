# Sprite AI (Engine-separated workspace)

Generated: 2025-12-29

Bu zip, **Unity/Unreal yaklaşımı** gibi **engine ayrı repo** olacak şekilde düzenlenmiş bir workspace içerir:

- `spriteai-engine/`  → çekirdek (Document/Tool/Command + Renderer API + Vulkan/CPU renderer)
- `spriteai-editor-desktop/` → Qt Editor (UI, docking, i18n, input)
- `spriteai-services/` → Backend + AI microservice
- `spriteai-infra/` → Docker Compose (PostgreSQL, RabbitMQ, pgAdmin, Jenkins)

> Not: Engine kodu editor’dan **bağımsızdır**. Editor, engine’i submodule gibi kullanır (CMake `add_subdirectory` veya package).
