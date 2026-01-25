# Sprite AI Services

Backend API and AI microservices for Sprite AI.

## Overview

This module contains:

- **Backend API** - Spring Boot REST API for user accounts, cloud storage, and project management
- **AI Services** - Microservices for AI-powered features (auto-coloring, style transfer, etc.)

## Structure

```
spriteai-services/
├── backend/          # Spring Boot main API
└── ai-service/       # AI/ML microservice (Python/FastAPI)
```

## Backend API

### Technology Stack

- Java 17+
- Spring Boot 3.x
- Spring Security (JWT authentication)
- Spring Data JPA
- PostgreSQL

### Running Locally

```bash
cd backend

# Using Maven
./mvnw spring-boot:run

# Or with Gradle
./gradlew bootRun
```

The API will be available at `http://localhost:8081`

### Configuration

Create `application-local.yml` for local settings:

```yaml
spring:
  datasource:
    url: jdbc:postgresql://localhost:5432/spriteai
    username: spriteai
    password: your-password

jwt:
  secret: your-secret-key
```

### API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/auth/login` | POST | User authentication |
| `/api/auth/register` | POST | User registration |
| `/api/projects` | GET | List user projects |
| `/api/projects` | POST | Create new project |
| `/api/projects/{id}` | GET | Get project details |

## AI Services

### Technology Stack

- Python 3.10+
- FastAPI
- PyTorch / TensorFlow
- RabbitMQ (for async processing)

### Features

- **Auto-Coloring**: Automatically colorize line art
- **Style Transfer**: Apply artistic styles to sprites
- **Upscaling**: AI-powered image upscaling
- **Background Removal**: Automatic background detection and removal

### Running Locally

```bash
cd ai-service

# Create virtual environment
python -m venv venv
source venv/bin/activate  # or venv\Scripts\activate on Windows

# Install dependencies
pip install -r requirements.txt

# Run service
uvicorn main:app --reload --port 8082
```

## Docker

Build and run with Docker:

```bash
# Build images
docker-compose build

# Run services
docker-compose up -d
```

## Development

### Prerequisites

- Java 17+ (for backend)
- Python 3.10+ (for AI service)
- PostgreSQL (or use Docker from spriteai-infra)
- RabbitMQ (or use Docker from spriteai-infra)

### Testing

```bash
# Backend tests
cd backend
./mvnw test

# AI service tests
cd ai-service
pytest
```

## License

MIT License - See [LICENSE](../LICENSE) file for details.
