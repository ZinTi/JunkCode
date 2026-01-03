<script setup lang="ts">
import { ref } from 'vue'

const currentYear = ref(new Date().getFullYear())

const footerLinks = ref([
  { name: '关于我们', url: '/about' },
  { name: '联系我们', url: '/contact' },
  { name: '隐私政策', url: '/privacy' },
  { name: '使用条款', url: '/terms' },
  { name: '帮助中心', url: '/help' }
])

const socialLinks = ref([
  { name: 'GitHub', icon: '🐙', url: 'https://github.com' },
  { name: '微博', icon: '📱', url: 'https://weibo.com' },
  { name: '知乎', icon: '💡', url: 'https://zhihu.com' },
  { name: 'B站', icon: '📺', url: 'https://bilibili.com' }
])
</script>

<template>
  <footer class="app-footer">
    <div class="footer-content">
      <div class="footer-section">
        <div class="footer-logo">
          <h3>长春工业大学</h3>
          <p class="footer-subtitle">Web 前端开发技术课程</p>
        </div>
        <p class="footer-description">
          期末结课作业<br>
          学习现代 Web 前端开发技术，<br>
          构建高效、美观、易用的用户界面。
        </p>
      </div>

      <div class="footer-section">
        <h4 class="footer-title">快速链接</h4>
        <ul class="footer-links">
          <li v-for="link in footerLinks" :key="link.name">
            <router-link :to="link.url" class="footer-link">
              {{ link.name }}
            </router-link>
          </li>
        </ul>
      </div>

      <div class="footer-section">
        <h4 class="footer-title">关注我们</h4>
        <div class="social-links">
          <a
            v-for="social in socialLinks"
            :key="social.name"
            :href="social.url"
            target="_blank"
            class="social-link"
            :title="social.name"
          >
            <span class="social-icon">{{ social.icon }}</span>
            <span class="social-name">{{ social.name }}</span>
          </a>
        </div>
      </div>

      <div class="footer-section">
        <h4 class="footer-title">其他信息</h4>
        <div class="course-info">
          <p>📚 专业: 计算机科学与技术</p>
          <p>👨‍🏫 班级: 223610</p>
          <p>📅 姓名: 张宇川</p>
          <p>📍 学号: 20221054</p>
        </div>
      </div>
    </div>

    <div class="footer-bottom">
      <div class="copyright">
        &copy; {{ currentYear }} 长春工业大学 - Web 前端开发技术课程. 保留所有权利.
      </div>
      <div class="footer-meta">
        <span>版本: v1.0.0</span>
        <span class="divider">|</span>
        <span>最后更新: 2025-12-31</span>
      </div>
    </div>
  </footer>
</template>

<style scoped>
.app-footer {
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
  color: #fff;
  padding: 3rem 1.5rem 1.5rem;
  margin-top: auto;
}

.footer-content {
  max-width: 1200px;
  margin: 0 auto;
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 2.5rem;
  padding-bottom: 2rem;
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
}

.footer-section {
  display: flex;
  flex-direction: column;
  gap: 1.2rem;
}

.footer-logo h3 {
  font-size: 1.8rem;
  font-weight: 700;
  margin: 0;
  background: linear-gradient(45deg, #4cc9f0, #4361ee);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.footer-subtitle {
  color: #a5b4cb;
  font-size: 0.9rem;
  margin-top: 0.5rem;
}

.footer-description {
  color: #b0b7c3;
  font-size: 0.95rem;
  line-height: 1.6;
  margin: 0;
}

.footer-title {
  font-size: 1.1rem;
  font-weight: 600;
  margin: 0 0 1rem 0;
  color: #fff;
  position: relative;
  padding-bottom: 0.5rem;
}

.footer-title::after {
  content: '';
  position: absolute;
  bottom: 0;
  left: 0;
  width: 40px;
  height: 2px;
  background: linear-gradient(90deg, #4361ee, #4cc9f0);
  border-radius: 2px;
}

.footer-links {
  list-style: none;
  padding: 0;
  margin: 0;
  display: flex;
  flex-direction: column;
  gap: 0.8rem;
}

.footer-link {
  color: #b0b7c3;
  text-decoration: none;
  font-size: 0.95rem;
  transition: all 0.3s ease;
  display: inline-block;
  position: relative;
  padding-left: 1rem;
}

.footer-link::before {
  content: '›';
  position: absolute;
  left: 0;
  color: #4361ee;
  transition: transform 0.3s ease;
}

.footer-link:hover {
  color: #fff;
  transform: translateX(5px);
}

.footer-link:hover::before {
  transform: scale(1.2);
  color: #4cc9f0;
}

.social-links {
  display: flex;
  flex-direction: column;
  gap: 0.9rem;
}

.social-link {
  display: flex;
  align-items: center;
  gap: 0.8rem;
  color: #b0b7c3;
  text-decoration: none;
  font-size: 0.95rem;
  transition: all 0.3s ease;
  padding: 0.5rem 0.8rem;
  border-radius: 8px;
  background: rgba(255, 255, 255, 0.05);
}

.social-icon {
  font-size: 1.2rem;
  transition: transform 0.3s ease;
}

.social-link:hover {
  color: #fff;
  background: rgba(255, 255, 255, 0.1);
  transform: translateY(-2px);
}

.social-link:hover .social-icon {
  transform: scale(1.2);
}

.course-info {
  display: flex;
  flex-direction: column;
  gap: 0.8rem;
}

.course-info p {
  color: #b0b7c3;
  font-size: 0.95rem;
  margin: 0;
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.footer-bottom {
  max-width: 1200px;
  margin: 2rem auto 0;
  padding-top: 1.5rem;
  text-align: center;
  color: #a5b4cb;
  font-size: 0.9rem;
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.copyright {
  font-size: 0.95rem;
}

.footer-meta {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 1rem;
  flex-wrap: wrap;
}

.divider {
  color: rgba(255, 255, 255, 0.2);
}

/* 响应式设计 */
@media (max-width: 768px) {
  .app-footer {
    padding: 2rem 1rem 1rem;
  }

  .footer-content {
    grid-template-columns: 1fr;
    gap: 2rem;
  }

  .footer-section {
    text-align: center;
  }

  .footer-title::after {
    left: 50%;
    transform: translateX(-50%);
  }

  .footer-link {
    padding-left: 0;
    justify-content: center;
  }

  .footer-link::before {
    display: none;
  }

  .footer-bottom {
    text-align: center;
  }

  .footer-meta {
    flex-direction: column;
    gap: 0.5rem;
  }

  .divider {
    display: none;
  }
}

@media (max-width: 480px) {
  .social-link {
    justify-content: center;
  }
}
</style>
