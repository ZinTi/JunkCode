<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import WaterMark from '../components/common/WaterMark.vue'
import LineEdit from '../components/common/LineEdit.vue'
import type { LoginRequest, LoginResponse } from '../types/api'

// 获取当前路由对象
const route = useRoute()
const router = useRouter()

// 水印文字
const watermark_text = computed(() => route.meta.watermark_text as string)

// 表单数据
const account = ref('') // 合并的输入框：可以是 username 或 email
const password = ref('')

// 验证状态
const accountValid = ref(false)
const passwordValid = ref(false)
const isLoading = ref(false)
const loginMessage = ref('') // 用于显示登录结果消息

// 判断输入的是邮箱还是用户名
const isEmail = (value: string): boolean => {
  const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/
  return emailRegex.test(value.trim())
}

// 账号验证函数（合并用户名和邮箱验证）
const validateAccount = (value: string) => {
  if (!value.trim()) {
    return { isValid: false, message: '账号不能为空' }
  }

  // 如果是邮箱格式，按邮箱验证
  if (isEmail(value)) {
    return { isValid: true }
  } else {
    // 否则按用户名验证
    return validateUsername(value)
  }
}

// 用户名验证函数
const validateUsername = (value: string) => {
  if (value.length < 3) {
    return { isValid: false, message: '用户名长度至少3位' }
  }

  if (value.length > 20) {
    return { isValid: false, message: '用户名长度不能超过20位' }
  }

  if (!/^[a-zA-Z0-9_]+$/.test(value)) {
    return { isValid: false, message: '用户名只能包含字母、数字和下划线' }
  }

  return { isValid: true }
}

// 密码验证函数
const validatePassword = (value: string) => {
  if (!value.trim()) {
    return { isValid: false, message: '密码不能为空' }
  }

  return { isValid: true }
}

// 处理验证状态变化
const handleValidityChange = (field: string, isValid: boolean) => {
  if (field === 'account') {
    accountValid.value = isValid
  } else if (field === 'password') {
    passwordValid.value = isValid
  }
}

// 清空本地存储中的登录数据
const clearLocalStorage = () => {
  const keys = ['gender', 'email', 'nickname', 'username', 'timestamp', 'auth_method', 'user_id', 'session_id']
  keys.forEach(key => {
    localStorage.removeItem(key)
  })
}

// 将登录响应数据保存到本地存储
const saveToLocalStorage = (data: LoginResponse) => {
  clearLocalStorage() // 先清空之前的存储

  // 遍历响应数据，排除message字段，将其他字段存储到localStorage
  Object.entries(data).forEach(([key, value]) => {
    if (key !== 'message') {
      localStorage.setItem(key, String(value))
    }
  })
}

// 处理登录
const handleLogin = async () => {
  if (!accountValid.value || !passwordValid.value) {
    loginMessage.value = '请填写正确的账号和密码'
    return
  }

  // 构建请求数据
  const loginData: LoginRequest = {
    password: password.value.trim()
  }

  // 根据输入内容判断是用户名还是邮箱
  if (isEmail(account.value.trim())) {
    loginData.email = account.value.trim()
  } else {
    loginData.username = account.value.trim()
  }

  isLoading.value = true
  loginMessage.value = ''

  try {
    // 发送登录请求
    const response = await fetch('/api/auth/', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(loginData)
    })

    const data: LoginResponse = await response.json()
    // console.log('登录响应数据:', data) // 打印响应数据，以便调试

    if (response.ok) {
      // 登录成功
      loginMessage.value = data.message

      // 保存数据到本地存储
      saveToLocalStorage(data)

      // 延迟跳转，让用户看到成功消息
      setTimeout(() => {
        router.push('/')
      }, 1500)
    } else {
      // 登录失败
      loginMessage.value = data.message || '登录失败，请检查账号和密码'

      // 清空本地存储
      clearLocalStorage()
    }
  } catch (error) {
    console.error('登录请求失败:', error)
    loginMessage.value = '网络错误，请稍后重试'

    // 清空本地存储
    clearLocalStorage()
  } finally {
    isLoading.value = false
  }
}

// 组件挂载时清空可能的旧登录数据
onMounted(() => {
  clearLocalStorage()
})
</script>

<template>
  <WaterMark
    :text="watermark_text"
    :fontSize="18"
    fontColor="#FFFFFF"
    :opacity="0.50"
    :rotate="-30"
  >
    <div class="auth-layout">
      <main class="auth-content">
        <router-view />

        <div class="top-form">
          <h1>登录系统</h1>
          <img src="../assets/logo.png" alt="logo" width="128px"/>
        </div>

        <div class="form-group">
          <!-- 登录结果消息显示 -->
          <div v-if="loginMessage" class="login-message" :class="{ 'success-message': loginMessage.includes('成功') }">
            {{ loginMessage }}
          </div>

          <!-- 1. 合并的用户名/邮箱输入 -->
          <LineEdit
            v-model="account"
            label="Account*"
            placeholder="用户名或邮箱"
            type="text"
            :validator="validateAccount"
            validate-on-input
            @validity-change="handleValidityChange('account', $event)"
          />
          <p class="hint-text">可以使用用户名或邮箱登录</p>

          <!-- 2. 密码输入 -->
          <LineEdit
            v-model="password"
            label="Password*"
            placeholder="密码"
            type="password"
            :validator="validatePassword"
            @validity-change="handleValidityChange('password', $event)"
          />
          <p class="hint-text">请输入密码</p>

          <!-- 提交按钮 -->
          <div class="control-panel">
            <button
              type="button"
              class="submit-btn"
              @click="handleLogin"
              :disabled="!accountValid || !passwordValid || isLoading"
            >
              <span v-if="isLoading">登录中...</span>
              <span v-else>登录 ></span>
            </button>
          </div>

          <div class="to-signup">
            <p>第一次使用？<router-link to="/signup">注册</router-link></p>
          </div>
        </div>

      </main>
    </div>
  </WaterMark>
</template>

<style scoped>
.auth-layout {
  min-height: 100vh;
  background: linear-gradient(135deg, white 0%, white 100%);
}

.auth-content {
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #0D1117;
  min-height: 100vh;
  padding: 20px;
}

.top-form {
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 2px 20px;
  align-items: center;
  justify-content: center;
  color: #eee;
}

.top-form h1 {
  margin: 0;
}

.form-group {
  padding: 20px;
  background-color: #f9f9f9;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
  max-width: 400px;
  width: 100%;
}

.form-group p {
  margin-top: 0;
  margin-bottom: 15px;
  color: #444;
  font-size: 12px;
}

.hint-text {
  margin-top: 4px;
  margin-bottom: 16px;
  color: #666;
  font-size: 12px;
}

.submit-btn {
  background-color: #00BD7E;
  color: white;
  width: 300px;
  height: 48px;
  border: none;
  border-radius: 6px;
  padding: 0;
  font-size: 16px;
  font-weight: 500;
  cursor: pointer;
  transition: background-color 0.3s;
}

.submit-btn:hover:not(:disabled) {
  background-color: #00a86b;
}

.submit-btn:disabled {
  background-color: #cccccc;
  cursor: not-allowed;
}

.control-panel {
  display: flex;
  flex-direction: column;
  padding: 2px;
  align-items: center;
  margin-top: 10px;
}

.to-signup {
  text-align: right;
  margin-top: 20px;
  width: 100%;
}

.to-signup p {
  font-size: 14px;
  margin: 0;
}

.to-signup a {
  color: #00BD7E;
  text-decoration: none;
}

.to-signup a:hover {
  text-decoration: underline;
}

.login-message {
  padding: 10px;
  margin-bottom: 15px;
  border-radius: 4px;
  text-align: center;
  font-size: 14px;
  background-color: #ffebee;
  color: #c62828;
  border: 1px solid #ffcdd2;
}

.success-message {
  background-color: #e8f5e9;
  color: #2e7d32;
  border: 1px solid #c8e6c9;
}
</style>
