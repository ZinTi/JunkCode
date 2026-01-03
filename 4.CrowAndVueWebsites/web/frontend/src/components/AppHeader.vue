<!-- AppHeader.vue -->

<script setup lang="ts">
// import type router from '@/router'
import { ref, computed, onMounted } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import SearchBox from '../components/common/SearchBox.vue'
import UserStatus from '../components/UserStatus.vue'

const router = useRouter()

const handleSearch = (value: string) => {
  console.log('搜索内容:', value)
  // TODO: 实际的搜索逻辑
}

const handleLogin = () => {
  console.log('登录事件触发')
  router.push('/login')
}

const handleLogout = () => {
  console.log('退出登录事件触发')
}

const handleRegister = () => {
  console.log('注册事件触发')
  router.push('/signup')
}
</script>

<template>
  <header class="app-header">
    <div class="header-left">
      <div class="logo">
        <img src="../assets/logo.png" alt="logo" width="64px"/>
      </div>

      <nav class="nav">
        <router-link to="/" class="nav-link">首页</router-link>
        <router-link to="/about" class="nav-link">关于</router-link>
        <router-link to="/contact" class="nav-link">联系</router-link>
      </nav>
    </div>

    <div class="header-center">
      <SearchBox
        placeholder="搜索内容..."
        size="medium"
        rounded
        show-icon
        auto-focus
        @search="handleSearch"
      />
    </div>

    <div class="header-right">
      <UserStatus
        :show-avatar="true"
        :compact="false"
        @logout="handleLogout"
      />

      <div class="auth-buttons">
        <button class="auth-button login-button" @click="handleLogin">登录</button>
        <button class="auth-button signup-button" @click="handleRegister">注册</button>
      </div>
    </div>
  </header>
</template>

<style scoped>
.app-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 10px 20px;
  background-color: #fff;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  gap: 20px;
}

.header-left {
  display: flex;
  align-items: center;
  gap: 40px;
  flex-shrink: 0;
}

.header-center {
  flex: 1;
  max-width: 600px;
  min-width: 200px;
}

.header-right {
  display: flex;
  align-items: center;
  gap: 20px;
  flex-shrink: 0;
}

.logo {
  display: flex;
  align-items: center;
}

.nav {
  display: flex;
  align-items: center;
  gap: 32px;
}

.nav-link {
  font-size: 16px;
  font-weight: 500;
  color: #555;
  text-decoration: none;
  padding: 8px 0;
  position: relative;
  transition: all 0.3s ease;
  white-space: nowrap;
}

.nav-link:hover {
  color: #3584e4;
}

.nav-link::after {
  content: '';
  position: absolute;
  bottom: 0;
  left: 0;
  width: 0;
  height: 2px;
  background-color: #3584e4;
  transition: width 0.3s ease;
}

.nav-link:hover::after {
  width: 100%;
}

.nav-link.router-link-active {
  color: #1c71d8;
  font-weight: 600;
}

.nav-link.router-link-active::after {
  width: 100%;
  background-color: #1c71d8;
}

.auth-buttons {
  display: flex;
  align-items: center;
  gap: 12px;
}

.auth-button {
  padding: 10px 24px;
  border-radius: 8px;
  font-size: 14px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  border: none;
  white-space: nowrap;
}

.login-button {
  background-color: #3584e4;
  color: white;
  box-shadow: 0 2px 4px rgba(53, 132, 228, 0.3);
}

.login-button:hover {
  background-color: #1c71d8;
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(53, 132, 228, 0.4);
}

.signup-button {
  background-color: transparent;
  color: #241f31;
  border: 1px solid rgba(0, 0, 0, 0.1);
}

.signup-button:hover {
  background-color: rgba(0, 0, 0, 0.04);
  transform: translateY(-2px);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

/* 响应式调整 */
@media (max-width: 1024px) {
  .header-left {
    gap: 24px;
  }

  .nav {
    gap: 20px;
  }

  .header-center {
    max-width: 400px;
  }
}

@media (max-width: 768px) {
  .app-header {
    flex-wrap: wrap;
    gap: 15px;
  }

  .header-left {
    order: 1;
    flex: 1;
  }

  .header-center {
    order: 3;
    flex-basis: 100%;
    max-width: 100%;
  }

  .header-right {
    order: 2;
  }

  .nav {
    gap: 15px;
  }
}
</style>
