package com.spriteai.backend.api;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Map;

@RestController
public class HealthController {

    @GetMapping("/api/health")
    public Map<String, String> getHealth() {
        return Map.of(
                "service", "SpriteAI Backend",
                "status", "UP"
        );
    }
}
