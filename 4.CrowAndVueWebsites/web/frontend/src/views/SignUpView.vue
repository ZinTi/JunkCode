<script setup lang="ts">
import { ref, computed, reactive } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import WaterMark from '../components/common/WaterMark.vue'
import LineEdit from '../components/common/LineEdit.vue'
import type { RegisterRequest, RegisterResponse } from '../types/api'

// 获取当前路由对象和路由器
const route = useRoute()
const router = useRouter()

// 水印文字
const watermark_text = computed(() => route.meta.watermark_text as string)

// 1.1 表单数据
const email = ref('')
const password = ref('')
const username = ref('')
const nickname = ref('')
const gender = ref('male') // 默认选择男性
const disabledField = ref('已禁用字段的值')

// 1.2 验证状态
const emailValid = ref(false)
const passwordValid = ref(false)
const usernameValid = ref(false)
const nicknameValid = ref(false)

// 1.3 注册状态
const registerState = reactive({
  isLoading: false,
  errorMessage: '',
  successMessage: ''
})

// 2.1 邮箱验证函数
const validateEmail = (value: string) => {
  if (!value.trim()) {
    return { isValid: false, message: '邮箱不能为空' }
  }

  const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/
  if (!emailRegex.test(value)) {
    return { isValid: false, message: '邮箱格式不正确' }
  }

  return { isValid: true }
}

// 2.2 密码验证函数
const validatePassword = (value: string) => {
  if (!value.trim()) {
    return { isValid: false, message: '密码不能为空' }
  }

  if (value.length < 6) {
    return { isValid: false, message: '密码长度至少6位' }
  }

  if (value.length > 18) {
    return { isValid: false, message: '密码长度至多18位' }
  }

  if (!/[A-Z]/.test(value)) {
    return { isValid: false, message: '密码必须包含大写字母' }
  }

  if (!/\d/.test(value)) {
    return { isValid: false, message: '密码必须包含数字' }
  }

  return { isValid: true }
}

// 2.3 用户名验证函数
const validateUsername = (value: string) => {
  if (!value.trim()) {
    return { isValid: false, message: '用户名不能为空' }
  }

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

// 2.4 昵称验证函数
const validateNickname = (value: string) => {
  if (!value.trim()) {
    return { isValid: false, message: '昵称不能为空' }
  }

  if (value.length < 2) {
    return { isValid: false, message: '昵称长度至少2位' }
  }

  if (value.length > 12) {
    return { isValid: false, message: '昵称长度不能超过12位' }
  }

  return { isValid: true }
}

// 处理验证状态变化
const handleValidityChange = (field: string, isValid: boolean) => {
  if (field === 'email') {
    emailValid.value = isValid
  } else if (field === 'password') {
    passwordValid.value = isValid
  } else if (field === 'username') {
    usernameValid.value = isValid
  } else if (field === 'nickname') {
    nicknameValid.value = isValid
  }
}

// 3.1 检查表单是否有效
const isFormValid = computed(() => {
  return emailValid.value &&
         passwordValid.value &&
         usernameValid.value &&
         nicknameValid.value
})

// 3.2 注册函数
const handleRegister = async () => {
  // 重置状态
  registerState.errorMessage = ''
  registerState.successMessage = ''

  // 验证表单
  if (!isFormValid.value) {
    registerState.errorMessage = '请完善表单信息'
    return
  }

  // 准备请求数据
  const registerData: RegisterRequest = {
    username: username.value,
    nickname: nickname.value,
    password: password.value,
    email: email.value,
    gender: gender.value === 'male' ? '男' : gender.value === 'female' ? '女' : '保密'
  }

  try {
    registerState.isLoading = true

    const response = await fetch('/api/users/', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(registerData)
    })

    const result: RegisterResponse = await response.json()

    if (response.ok) {
      registerState.successMessage = result.message || '注册成功！'

      // 注册成功，可以跳转到登录页面或显示成功信息
      setTimeout(() => {
        router.push('/login')
      }, 2000)
    } else {
      registerState.errorMessage = result.message || '注册失败，请稍后重试'
    }
  } catch (error) {
    console.error('注册请求失败:', error)
    registerState.errorMessage = '网络错误，请检查网络连接'
  } finally {
    registerState.isLoading = false
  }
}

// 3.3 重置表单
const resetForm = () => {
  email.value = ''
  password.value = ''
  username.value = ''
  nickname.value = ''
  gender.value = 'male'
  registerState.errorMessage = ''
  registerState.successMessage = ''
}
</script>

<template>
<WaterMark
  :text="watermark_text"
  :fontSize="18"
  fontColor="#0963FF"
  :opacity="0.50"
  :rotate="-30"
><!-- 使用水印组件 -->
  <div class="auth-layout">
    <!-- 可添加背景、logo等公共元素，但没有Header/Footer -->
    <main class="auth-content">
      <router-view />

      <div class="left-form">
        <h1>创建你的账号</h1>
        <img src="../assets/logo.png" alt="logo" />

      </div>

      <div class="right-form">
        <div class="to-login">
          <p>已经有账号了？<router-link to="/login">登录</router-link></p>
        </div>

        <div class="title">
          <h3>注册账号</h3>
        </div>

        <!-- 错误和成功消息提示 -->
        <div v-if="registerState.errorMessage" class="message error">
          {{ registerState.errorMessage }}
        </div>
        <div v-if="registerState.successMessage" class="message success">
          {{ registerState.successMessage }}
        </div>

        <div class="form-group">
          <!-- 1. 邮箱输入（合法性验证） -->
          <LineEdit
            v-model="email"
            label="Email*"
            placeholder="邮箱"
            type="email"
            :validator="validateEmail"
            validate-on-input
            @validity-change="handleValidityChange('email', $event)"
          />
          <p></p> <!-- space line -->

          <!-- 2. 密码输入（强度验证） -->
          <LineEdit
            v-model="password"
            label="Password*"
            placeholder="密码"
            type="password"
            :validator="validatePassword"
            @validity-change="handleValidityChange('password', $event)"
          />
          <p>Password should be at least 18 characters OR at least 6 characters including a number and a uppercase letter.</p> <!-- space line -->

          <!-- 3. 用户名输入（强度验证） -->
          <LineEdit
            v-model="username"
            label="Username*"
            placeholder="用户名"
            :validator="validateUsername"
            @validity-change="handleValidityChange('username', $event)"
          />
          <p>Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen.</p>

          <!-- 4. 昵称输入（强度验证） -->
          <LineEdit
            v-model="nickname"
            label="Nickname*"
            placeholder="昵称"
            :validator="validateNickname"
            @validity-change="handleValidityChange('nickname', $event)"
          />
          <p></p>

          <!-- 5. 性别单选按钮 -->
          <div>
            <label class="edit-label">Gender*</label>
            <br/>
            <input type="radio" id="male" name="gender" value="male" v-model="gender" checked>
            <label for="male" style="color: black;">男</label>
            &nbsp;
            <input type="radio" id="female" name="gender" value="female" v-model="gender">
            <label for="female" style="color: black;">女</label>
            &nbsp;
            <input type="radio" id="unknown" name="gender" value="unknown" v-model="gender">
            <label for="unknown" style="color: black;">保密</label>
          </div>
          <p></p>

          <!-- 6. 提交按钮 -->
          <div class="control-panel">
            <button
              type="button"
              class="submit-btn"
              @click="handleRegister"
              :disabled="!isFormValid || registerState.isLoading"
            >
              {{ registerState.isLoading ? '注册中...' : '注册 >' }}
            </button>
            <button type="button" class="reset-btn" @click="resetForm">
              重置
            </button>
            <p>创建账号，代表你同意<a>《服务协议》</a>与<a>《隐私条款》</a></p>
          </div>

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
  min-height: 100vh;
  /* padding: 20px; */
}

.description {
  color: #666;
  margin-bottom: 30px;
  font-size: 16px;
  line-height: 1.5;
}

.left-form {
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 2px 20px;
  background-color: #0D1117;
  width: 50%;
  align-items: center;
  justify-content: center;
  color: #eee;
}

.right-form {
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 2px 20px;
  background-color: #f9f9f9;
  width: 50%;
  align-items: center;
}

.to-login {
  text-align: right;
  margin-bottom: 20px;
  width: 100%;
  align-self: flex-start;
}

.title {
  /* text-align: center; */
}

.form-group {
  padding: 20px;
  background-color: white;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
  max-width: 400px;
}

.form-group p {
  margin-top: 0;
  margin-bottom: 15px;
  color: #444;
  font-size: 12px;
}

.edit-label {
  font-size: 14px;
  font-weight: 500;
  color: #333;
  margin-bottom: 6px;
  transition: color 0.2s ease;
}

.submit-btn {
  background-color: black;
  color: white;
  width: 300px;
  height: 48px;
  border: none;
  border-radius: 6px;
  padding: 0;
  font-size: 12px;
  cursor: pointer;
  margin-bottom: 10px;
}

.submit-btn:disabled {
  background-color: #ccc;
  cursor: not-allowed;
}

.reset-btn {
  background-color: #f5f5f5;
  color: #333;
  width: 300px;
  height: 36px;
  border: 1px solid #ddd;
  border-radius: 6px;
  font-size: 12px;
  cursor: pointer;
  margin-bottom: 10px;
}

.control-panel {
  display: flex;
  flex-direction: column;
  padding: 2px;
  align-items: center;
}

.control-panel p {
  margin-top: 2px;
  margin-bottom: 0;
}

.message {
  padding: 12px;
  border-radius: 6px;
  margin-bottom: 15px;
  width: 100%;
  max-width: 400px;
  text-align: center;
}

.message.error {
  background-color: #ffebee;
  color: #c62828;
  border: 1px solid #ffcdd2;
}

.message.success {
  background-color: #e8f5e9;
  color: #2e7d32;
  border: 1px solid #c8e6c9;
}

.controls {
  display: flex;
  gap: 12px;
  margin-bottom: 20px;
  flex-wrap: wrap;
}

.btn {
  padding: 10px 20px;
  border: none;
  border-radius: 6px;
  font-size: 16px;
  cursor: pointer;
  transition: all 0.2s ease;
  font-weight: 500;
}

.btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.btn:active {
  transform: translateY(0);
}

.btn-primary {
  background-color: #1976d2;
  color: white;
}

.btn-secondary {
  background-color: #f5f5f5;
  color: #333;
  border: 1px solid #ddd;
}

.btn-success {
  background-color: #2e7d32;
  color: white;
}

.validation-status {
  padding: 15px;
  background-color: #f8f9fa;
  border-radius: 6px;
  border-left: 4px solid #1976d2;
}

.validation-status h4 {
  margin-top: 0;
  margin-bottom: 10px;
  color: #333;
}

.validation-status ul {
  list-style: none;
  padding: 0;
  margin: 0;
}

.validation-status li {
  padding: 8px 0;
  border-bottom: 1px solid #eee;
  color: #666;
}

.validation-status li:last-child {
  border-bottom: none;
}

.validation-status li.valid {
  color: #2e7d32;
  font-weight: 500;
}
</style>
