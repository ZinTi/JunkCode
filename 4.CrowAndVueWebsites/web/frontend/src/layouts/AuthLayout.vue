<!-- layouts/AuthLayout.vue -->
<template>
  <div class="auth-layout">
    <!-- 仅包含页面内容，无Header/Footer -->
    <router-view />
  </div>
</template>

<style scoped>
.auth-layout {
  min-height: 100vh;
  background: #f5f5f5; /* 可选：设置登录/注册页背景 */
}
</style>
