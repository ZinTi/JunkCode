package com.gjj.entity;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;

@Setter
@Getter
public class ProjectConfig {
    private String project;
    private String version;
    private String copyleft;
    private String license;
    private String author;
    private LocalDateTime now;

    public ProjectConfig() {
    }

    public ProjectConfig(String project, String version, String copyleft, String license, String author, LocalDateTime now) {
        this.project = project;
        this.version = version;
        this.copyleft = copyleft;
        this.license = license;
        this.author = author;
        this.now = now;
    }
}
