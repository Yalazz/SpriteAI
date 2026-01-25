# Sprite AI Infrastructure

Docker Compose configuration for local development infrastructure.

## Services

| Service | Port | Description |
|---------|------|-------------|
| PostgreSQL | 5432 | Primary database |
| pgAdmin | 5050 | Database management UI |
| RabbitMQ | 5672, 15672 | Message queue (AMQP + Management UI) |
| Jenkins | 8080 | CI/CD server |

## Quick Start

### Prerequisites

- Docker Desktop installed and running
- Docker Compose (included with Docker Desktop)

### Starting Services

```bash
cd spriteai-infra

# Start all services
docker-compose up -d

# View logs
docker-compose logs -f

# Stop all services
docker-compose down
```

### Starting Individual Services

```bash
# Database only
docker-compose up -d postgres pgadmin

# Message queue only
docker-compose up -d rabbitmq
```

## Accessing Services

### PostgreSQL

- **Host**: localhost
- **Port**: 5432
- **Database**: spriteai
- **Username**: spriteai
- **Password**: (see docker-compose.yml)

Connection string:
```
postgresql://spriteai:password@localhost:5432/spriteai
```

### pgAdmin

- **URL**: http://localhost:5050
- **Email**: admin@spriteai.local
- **Password**: (see docker-compose.yml)

To connect to PostgreSQL from pgAdmin:
1. Add New Server
2. Name: `SpriteAI Local`
3. Host: `postgres` (Docker network name)
4. Port: `5432`
5. Username/Password: (see docker-compose.yml)

### RabbitMQ

- **AMQP URL**: amqp://localhost:5672
- **Management UI**: http://localhost:15672
- **Username**: guest
- **Password**: guest

### Jenkins

- **URL**: http://localhost:8080
- Initial admin password: Check container logs or `/var/jenkins_home/secrets/initialAdminPassword`

## Data Persistence

Data is stored in Docker volumes:
- `postgres_data` - PostgreSQL database files
- `rabbitmq_data` - RabbitMQ data
- `jenkins_data` - Jenkins configuration and jobs

To reset all data:
```bash
docker-compose down -v
```

## Configuration

Environment variables can be customized in `docker-compose.yml` or via `.env` file.

## Troubleshooting

### Port already in use
Check if another service is using the port:
```bash
lsof -i :5432  # Check PostgreSQL port
```

### Container won't start
Check logs:
```bash
docker-compose logs postgres
```

### Reset database
```bash
docker-compose down
docker volume rm spriteai-infra_postgres_data
docker-compose up -d
```

## License

MIT License - See [LICENSE](../LICENSE) file for details.
