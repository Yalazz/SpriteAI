# Sprite AI

Sprite AI; Vulkan tabanlı, C++/Qt ile yazılmış, Windows/macOS/Linux üzerinde çalışan,
AI destekli ve multiplayer özellikli bir grafik editörüdür.

Bu monorepo yapısında şunlar bulunmaktadır:

- `spriteai-desktop/`  → C++20 + Qt6 + Vulkan masaüstü editör iskeleti
- `spriteai-backend/`  → Spring Boot backend (PostgreSQL + Redis)
- `spriteai-ai/`       → FastAPI tabanlı AI servisi
- `spriteai-infra/`    → Docker Compose ile Postgres 17, pgAdmin, Redis, Backend, AI, Jenkins

Ayrıca `.github/workflows/` altında GitHub Actions CI iskeletleri bulunmaktadır.
