FROM eclipse-temurin:17-jdk

WORKDIR /app

# Maven kur
RUN apt-get update && apt-get install -y maven

# pom.xml → dependency indirme
COPY pom.xml .
RUN mvn -q dependency:go-offline

# kaynak kod
COPY src ./src

# build al
RUN mvn -q -DskipTests package

EXPOSE 8080

ENTRYPOINT ["java", "-jar", "target/spriteai-backend-0.0.1-SNAPSHOT.jar"]
