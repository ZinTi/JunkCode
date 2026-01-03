package com.gjj.controller.api;

import com.gjj.entity.ProjectConfig;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;

@RestController
@RequestMapping("/api/health")
class ConfigController {
    private static final Logger logger = LoggerFactory.getLogger(ConfigController.class);

    public ConfigController() {
        logger.info("ConfigController initialized!");
    }

    @GetMapping("/config")
    public ResponseEntity<ProjectConfig> sayHello() {
        logger.info("Received request to /api/hello"); // 添加日志

        return ResponseEntity.ok(
            new ProjectConfig(
                    "GJJ-ContributionModule",
                    "v1.0.0",
                    "2025",
                    "GPLv3",
                    null,
                    LocalDateTime.now()
            )
        );
    }

    @GetMapping("/hello")
    public String healthCheck() {
        return "Service is UP!";
    }

}
