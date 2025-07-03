package com.gjj;

import org.apache.catalina.Context;
import com.gjj.config.AppConfig;
import org.apache.catalina.Wrapper;
import org.apache.catalina.startup.Tomcat;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.context.support.AnnotationConfigWebApplicationContext;
import org.springframework.web.servlet.DispatcherServlet;
import org.apache.catalina.LifecycleState;

import java.io.File;

public class Application {
    private static final Logger logger = LoggerFactory.getLogger(Application.class);

    public static void main(String[] args) throws Exception {
        try {
            logger.info("Starting application...");

            // 1. 创建Tomcat实例
            Tomcat tomcat = new Tomcat();
            tomcat.setPort(8080);
            logger.info("Tomcat port set to 8080");

            // 2. 设置Web应用目录
            File webappDir = getWebappDirectory();
            logger.info("Using webapp directory: {}", webappDir.getAbsolutePath());

            // 3. 添加上下文
            Context context = tomcat.addContext("", webappDir.getAbsolutePath());
            logger.info("Tomcat context created");

            // 4. 创建Spring上下文
            AnnotationConfigWebApplicationContext appContext = new AnnotationConfigWebApplicationContext();
            appContext.register(AppConfig.class);
            logger.info("Spring context registered");

            // 5. 创建DispatcherServlet并关联上下文
            DispatcherServlet servlet = new DispatcherServlet(appContext);
            Wrapper wrapper = Tomcat.addServlet(context, "dispatcher", servlet);
            wrapper.setLoadOnStartup(1);  // 确保在启动时初始化
            // context.addServletMappingDecoded("/*", "dispatcher");
            context.addServletMappingDecoded("/", "dispatcher");
            logger.info("DispatcherServlet configured");

            // 6. 启动Tomcat
            logger.info("Starting Tomcat server...");
            tomcat.start();

            // 7. 验证启动状态
            if (tomcat.getServer().getState() != LifecycleState.STARTED) {
                logger.error("Tomcat failed to start! State: {}", tomcat.getServer().getState());
                return;
            }

            int port = tomcat.getConnector().getLocalPort();
            logger.info("Tomcat successfully started on port: {}", port);
            logger.info("Application is running. Press Ctrl+C to exit.");

            // 8. 保持运行
            tomcat.getServer().await();

        } catch (Exception e) {
            logger.error("Application startup failed", e);
        }

    }

    private static File getWebappDirectory() {
        // 尝试创建临时目录
        File tempDir = new File(System.getProperty("java.io.tmpdir"), "gjj-webapp");
        if (!tempDir.exists()) {
            tempDir.mkdirs();
        }

        // 创建必要的子目录
        new File(tempDir, "WEB-INF").mkdirs();
        return tempDir;
    }

}