# Sprite AI Backend

Spring Boot REST API for Sprite AI.

## Overview

This is the main backend service providing:

- User authentication and authorization (JWT)
- Project and asset management
- Cloud storage integration
- Real-time collaboration (WebSocket)

## Technology Stack

- **Java 17+**
- **Spring Boot 3.x**
- **Spring Security** - JWT-based authentication
- **Spring Data JPA** - Database access
- **PostgreSQL** - Primary database
- **RabbitMQ** - Message queue for async tasks

## Getting Started

### Prerequisites

- Java 17 or higher
- Maven or Gradle
- PostgreSQL (running locally or via Docker)

### Running

```bash
# Start infrastructure (PostgreSQL, RabbitMQ)
cd ../spriteai-infra
docker-compose up -d postgres rabbitmq

# Run the backend
cd ../spriteai-services/backend
./mvnw spring-boot:run
```

The API will be available at `http://localhost:8081`

### Configuration

Default configuration is in `src/main/resources/application.yml`.

For local development, create `application-local.yml`:

```yaml
spring:
  datasource:
    url: jdbc:postgresql://localhost:5432/spriteai
    username: spriteai
    password: your-password

  jpa:
    hibernate:
      ddl-auto: update

jwt:
  secret: your-256-bit-secret-key
  expiration: 86400000  # 24 hours
```

Run with local profile:
```bash
./mvnw spring-boot:run -Dspring.profiles.active=local
```

## API Documentation

When running, Swagger UI is available at:
- http://localhost:8081/swagger-ui.html

## Project Structure

```
backend/
├── src/main/java/
│   └── com/spriteai/
│       ├── config/        # Configuration classes
│       ├── controller/    # REST controllers
│       ├── service/       # Business logic
│       ├── repository/    # Data access
│       ├── model/         # Entity classes
│       ├── dto/           # Data transfer objects
│       └── security/      # JWT, authentication
└── src/main/resources/
    └── application.yml    # Configuration
```

## Testing

```bash
# Run all tests
./mvnw test

# Run with coverage
./mvnw test jacoco:report
```

## Building

```bash
# Build JAR
./mvnw clean package

# Build without tests
./mvnw clean package -DskipTests

# Run the JAR
java -jar target/backend-*.jar
```

## Docker

```bash
# Build image
docker build -t spriteai-backend .

# Run container
docker run -p 8081:8081 spriteai-backend
```

## License

MIT License - See [LICENSE](../../LICENSE) file for details.
