# Sprite AI - Infra

Bu klasör, lokal geliştirme ortamını Docker Compose ile ayağa kaldırmak için kullanılır.

Servisler:
- PostgreSQL 17
- pgAdmin 4
- Redis
- SpriteAI Backend
- SpriteAI AI Service
- Jenkins (CI sunucusu)

Çalıştırmak için:

```bash
cd spriteai-infra
docker compose up --build
```

pgAdmin için:
- URL: http://localhost:5050
- Kullanıcı: admin@spriteai.local
- Parola: admin123

Jenkins için:
- URL: http://localhost:8081
- İlk kurulumda admin parolasını konteyner loglarından almanız gerekir:
  - `docker logs spriteai-jenkins` komutu ile görebilirsiniz.
