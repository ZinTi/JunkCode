package com.gjj.config;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Import;
import org.springframework.core.io.ClassPathResource;
import org.springframework.core.io.Resource;
import org.springframework.web.servlet.config.annotation.*;

@Configuration
@EnableWebMvc
@ComponentScan(basePackages = {
        "com.gjj",
        "com.gjj.controller",
        "com.gjj.controller.api"
})
@Import({
        DataSourceConfig.class,
        MyBatisConfig.class,
        MvcConfig.class,
        JacksonConfig.class
})
public class AppConfig implements WebMvcConfigurer {    // 实现 WebMvcConfigurer 接口

    private static final Logger logger = LoggerFactory.getLogger(AppConfig.class);

    public AppConfig() {
        logger.info("AppConfig initialized!");
        // debug: 验证文件是否存在
        Resource resource = new ClassPathResource("static/index.html");
        logger.info("index.html exists: {}", resource.exists());
    }

    // 1. 静态资源处理
    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("/**")  // 匹配所有路径
                .addResourceLocations("classpath:/static/"); // 映射到静态资源根目录
        /*
        // 1. 仅处理静态资源请求，排除API路径
        registry.addResourceHandler(
                        "/css/**",
                        "/js/**",
                        "/images/**",
                        "/fonts/**",
                        "/**.html",
                        "/**.ico",
                        "/**.txt"
                )
                .addResourceLocations("classpath:/static/");

        // 2. 单独处理根路径
        /*registry.addResourceHandler("/")
                .addResourceLocations("classpath:/static/index.html");*/
    }

    // 更安全的根路径处理
    @Override
    public void addViewControllers(ViewControllerRegistry registry) {
        registry.addViewController("/").setViewName("forward:/index.html");
    }

}