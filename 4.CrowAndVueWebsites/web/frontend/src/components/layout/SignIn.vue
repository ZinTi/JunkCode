<template>
  <div class="example-container">
    <h1>LineEdit 输入框控件示例</h1>
    <p class="description">这是一个使用Vue3组合式API和TypeScript创建的LineEdit组件示例。</p>

    <div class="example-form">
      <!-- 示例1: 邮箱输入 -->
      <div class="form-group">
        <h3>示例1: 邮箱验证</h3>
        <LineEdit
          v-model="email"
          label="Email"
          placeholder="请输入邮箱地址"
          type="email"
          :validator="validateEmail"
          validate-on-input
          @validity-change="handleValidityChange('email', $event)"
        />
        <div class="current-value">当前值: {{ email || '(空)' }}</div>
      </div>

      <!-- 示例2: 密码输入 -->
      <div class="form-group">
        <h3>示例2: 密码强度验证</h3>
        <LineEdit
          v-model="password"
          label="Password"
          placeholder="请输入密码"
          type="password"
          :validator="validatePassword"
          @validity-change="handleValidityChange('password', $event)"
        />
        <div class="current-value">当前值: {{ password ? '•'.repeat(password.length) : '(空)' }}</div>
      </div>

      <!-- 示例3: 用户名输入 -->
      <div class="form-group">
        <h3>示例3: 用户名验证</h3>
        <LineEdit
          v-model="username"
          label="Username"
          :validator="validateUsername"
          @validity-change="handleValidityChange('username', $event)"
        />
        <div class="current-value">当前值: {{ username || '(空)' }}</div>
      </div>

      <!-- 示例4: 禁用状态 -->
      <div class="form-group">
        <h3>示例4: 禁用状态</h3>
        <LineEdit
          v-model="disabledField"
          label="Disabled Field"
          placeholder="此输入框已被禁用"
          :disabled="true"
        />
      </div>

      <!-- 控制面板 -->
      <div class="control-panel">
        <h3>控制面板</h3>
        <div class="controls">
          <button
            class="btn"
            :class="{ 'btn-primary': !showAllValid, 'btn-success': showAllValid }"
            @click="validateAll"
          >
            {{ showAllValid ? '✅ 全部验证通过' : '验证所有字段' }}
          </button>
          <button class="btn btn-secondary" @click="clearAll">清空所有字段</button>
        </div>

        <div class="validation-status">
          <h4>验证状态:</h4>
          <ul>
            <li :class="{ 'valid': emailValid }">邮箱: {{ emailValid ? '✓ 有效' : '✗ 无效' }}</li>
            <li :class="{ 'valid': passwordValid }">密码: {{ passwordValid ? '✓ 有效' : '✗ 无效' }}</li>
            <li :class="{ 'valid': usernameValid }">用户名: {{ usernameValid ? '✓ 有效' : '✗ 无效' }}</li>
          </ul>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import LineEdit from '../common/LineEdit.vue'

// 表单数据
const email = ref('')
const password = ref('')
const username = ref('')
const disabledField = ref('已禁用字段的值')

// 验证状态
const emailValid = ref(false)
const passwordValid = ref(false)
const usernameValid = ref(false)

// 计算是否全部验证通过
const showAllValid = computed(() =>
  emailValid.value && passwordValid.value && usernameValid.value
)

// 邮箱验证函数
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

// 密码验证函数
const validatePassword = (value: string) => {
  if (!value.trim()) {
    return { isValid: false, message: '密码不能为空' }
  }

  if (value.length < 6) {
    return { isValid: false, message: '密码长度至少6位' }
  }

  if (!/[A-Z]/.test(value)) {
    return { isValid: false, message: '密码必须包含大写字母' }
  }

  if (!/\d/.test(value)) {
    return { isValid: false, message: '密码必须包含数字' }
  }

  return { isValid: true }
}

// 用户名验证函数
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

// 处理验证状态变化
const handleValidityChange = (field: string, isValid: boolean) => {
  if (field === 'email') {
    emailValid.value = isValid
  } else if (field === 'password') {
    passwordValid.value = isValid
  } else if (field === 'username') {
    usernameValid.value = isValid
  }
}

// 验证所有字段
const validateAll = () => {
  // 触发所有字段的验证
  const emailInput = document.querySelector('#email-field') as HTMLInputElement
  const passwordInput = document.querySelector('#password-field') as HTMLInputElement
  const usernameInput = document.querySelector('#username-field') as HTMLInputElement

  if (emailInput) emailInput.blur()
  if (passwordInput) passwordInput.blur()
  if (usernameInput) usernameInput.blur()

  // 在实际项目中，可以通过ref调用组件暴露的方法
  console.log('验证所有字段...')
}

// 清空所有字段
const clearAll = () => {
  email.value = ''
  password.value = ''
  username.value = ''
  emailValid.value = false
  passwordValid.value = false
  usernameValid.value = false
  console.log('已清空所有字段')
}
</script>

<style scoped>
.example-container {
  max-width: 800px;
  margin: 0 auto;
  padding: 20px;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
}

h1 {
  color: #333;
  margin-bottom: 10px;
}

.description {
  color: #666;
  margin-bottom: 30px;
  font-size: 16px;
  line-height: 1.5;
}

.example-form {
  display: flex;
  flex-direction: column;
  gap: 30px;
  padding: 20px;
  background-color: #f9f9f9;
  border-radius: 12px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

.form-group {
  padding: 20px;
  background-color: white;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
}

.form-group h3 {
  margin-top: 0;
  margin-bottom: 15px;
  color: #444;
  font-size: 18px;
}

.current-value {
  margin-top: 10px;
  font-size: 14px;
  color: #666;
  font-family: 'Courier New', monospace;
  padding: 8px 12px;
  background-color: #f5f5f5;
  border-radius: 4px;
  border-left: 3px solid #1976d2;
}

.control-panel {
  padding: 20px;
  background-color: white;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
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
