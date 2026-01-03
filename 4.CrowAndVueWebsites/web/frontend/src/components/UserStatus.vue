<script lang="ts" setup>
import { ref, onMounted, reactive, watch } from 'vue'
import { useRouter } from 'vue-router'

import genderIconMale from '../assets/gender-male96.png'
import genderIconFemale from '../assets/gender-female96.png'
import genderIconUnknown from '../assets/gender-unknown96.png'

// 定义用户信息接口
interface UserInfo {
  username?: string
  nickname?: string
  gender?: string
  [key: string]: any // 添加索引签名
}

// 定义组件接收的属性
interface Props {
  compact?: boolean
}

const props = withDefaults(defineProps<Props>(), {
  compact: false
})

// 初始化路由
const router = useRouter()

// 响应式数据
const isLoggedIn = ref(false)
const isLoading = ref(true)
const userInfo = reactive<UserInfo>({})

// 性别图标映射
const genderIcons = {
  '男': genderIconMale,
  '女': genderIconFemale,
  '未知': genderIconUnknown
}

// 获取性别图标路径
const getGenderIcon = (gender?: string) => {
  if (!gender) return genderIcons['未知']

  // 检查是否是有效的性别值
  if (gender === '男' || gender === '女' || gender === '未知') {
    return genderIcons[gender]
  }

  return genderIcons['未知']
}

// 获取性别显示的文本
const getGenderText = (gender?: string) => {
  if (!gender) return '未知'

  if (gender === '男' || gender === '女' || gender === '未知') {
    return gender
  }

  return '未知'
}

// 验证会话有效性
const validateSession = async (): Promise<boolean> => {
  const sessionId = localStorage.getItem('session_id')

  if (!sessionId) {
    return false
  }

  try {
    const response = await fetch('/api/auth/', {
      method: 'GET',
      headers: {
        'session_id': sessionId
      }
    })

    return response.status === 204
  } catch (error) {
    console.error('验证会话失败:', error)
    return false
  }
}

// 获取用户信息
const fetchUserInfo = async () => {
  const sessionId = localStorage.getItem('session_id')

  if (!sessionId) {
    isLoggedIn.value = false
    isLoading.value = false
    router.push('/login')
    return
  }

  try {
    const isValid = await validateSession()

    if (!isValid) {
      isLoggedIn.value = false
      isLoading.value = false
      router.push('/login')
      return
    }

    // 会话有效，从本地存储读取用户信息
    const username = localStorage.getItem('username')
    const nickname = localStorage.getItem('nickname')
    const gender = localStorage.getItem('gender')

    // 更新用户信息
    userInfo.username = username || undefined
    userInfo.nickname = nickname || undefined
    userInfo.gender = gender || '未知'

    isLoggedIn.value = true
  } catch (error) {
    console.error('获取用户信息失败:', error)
    isLoggedIn.value = false
    router.push('/login')
  } finally {
    isLoading.value = false
  }
}

// 处理退出登录
const handleLogout = async () => {
  const sessionId = localStorage.getItem('session_id')

  if (sessionId) {
    try {
      await fetch('/api/auth/', {
        method: 'DELETE',
        headers: {
          'session_id': sessionId
        }
      })
    } catch (error) {
      console.error('退出登录请求失败:', error)
    }
  }

  // 清除本地存储
  const keysToRemove = ['session_id', 'username', 'nickname', 'gender']
  keysToRemove.forEach(key => localStorage.removeItem(key))

  // 重置用户信息 - 修复TypeScript错误
  Object.keys(userInfo).forEach(key => {
    if (key in userInfo) {
      delete userInfo[key]
    }
  })

  isLoggedIn.value = false

  // 跳转到登录页
  router.push('/login')
}

// 初始化检查
onMounted(() => {
  fetchUserInfo()

  // 监听storage变化
  window.addEventListener('storage', (event) => {
    if (event.key === 'session_id') {
      fetchUserInfo()
    }
  })
})

// 监听session_id变化
watch(() => localStorage.getItem('session_id'), () => {
  fetchUserInfo()
})
</script>

<template>
  <div class="user-status-container">
    <div v-if="isLoading" class="loading-state">
      <div class="loading-spinner"></div>
      <span v-if="!compact" class="loading-text">加载中...</span>
    </div>

    <div v-else-if="isLoggedIn" class="logged-in-state">
      <div class="user-info-container" :class="{ 'compact': compact }">
        <!-- 用户信息 -->
        <div class="user-details">
          <span class="user-nickname">
            {{ userInfo.nickname || userInfo.username || '用户' }}
          </span>
          <div v-if="!compact" class="user-extra">
            <span class="user-username">@{{ userInfo.username }}</span>
            <!-- 性别图标 -->
            <div class="gender-info" :title="getGenderText(userInfo.gender)">
              <img
                :src="getGenderIcon(userInfo.gender)"
                :alt="getGenderText(userInfo.gender)"
                class="gender-icon"
              />
            </div>
          </div>
        </div>

        <!-- 退出登录按钮 -->
        <button class="logout-button" @click="handleLogout">
          <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"></path>
            <polyline points="16 17 21 12 16 7"></polyline>
            <line x1="21" y1="12" x2="9" y2="12"></line>
          </svg>
          <span v-if="!compact">退出登录</span>
        </button>
      </div>
    </div>

    <div v-else class="logged-out-state">
      <div class="login-prompt" :class="{ 'compact': compact }">
        <span v-if="!compact" class="prompt-text">请登录</span>
      </div>
    </div>
  </div>
</template>

<style scoped>
.user-status-container {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Noto Sans', Helvetica, Arial, sans-serif;
  position: relative;
}

/* 加载状态 */
.loading-state {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 8px 16px;
  color: #77767b;
  opacity: 0.8;
}

.loading-spinner {
  width: 20px;
  height: 20px;
  border: 2px solid #f3f3f3;
  border-top: 2px solid #3584e4;
  border-radius: 50%;
  animation: spin 1s linear infinite;
}

.loading-text {
  font-size: 14px;
  font-weight: 400;
}

@keyframes spin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}

/* 已登录状态 */
.user-info-container {
  display: flex;
  align-items: center;
  gap: 16px;
  padding: 8px 16px;
  border-radius: 12px;
  background-color: transparent;
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  user-select: none;
}

.user-info-container.compact {
  padding: 6px 12px;
  gap: 12px;
}

.user-details {
  display: flex;
  flex-direction: column;
  min-width: 0;
}

.user-nickname {
  font-weight: 600;
  font-size: 15px;
  color: #241f31;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  max-width: 120px;
}

.user-extra {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-top: 2px;
}

.user-username {
  font-size: 12px;
  color: #77767b;
  opacity: 0.8;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  max-width: 120px;
}

/* 性别图标样式 */
.gender-info {
  display: flex;
  align-items: center;
  cursor: default;
}

.gender-icon {
  width: 16px;
  height: 16px;
  opacity: 0.7;
  object-fit: contain;
  transition: opacity 0.2s ease;
}

.gender-icon:hover {
  opacity: 1;
}

.logout-button {
  display: flex;
  align-items: center;
  gap: 6px;
  padding: 8px 12px;
  border: 1px solid rgba(224, 27, 36, 0.3);
  border-radius: 8px;
  background-color: #f6f5f4;
  color: #e01b24;
  font-size: 14px;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  white-space: nowrap;
}

.logout-button:hover {
  background-color: rgba(224, 27, 36, 0.08);
  transform: translateY(-1px);
  box-shadow: 0 2px 4px rgba(224, 27, 36, 0.1);
}

.logout-button:active {
  transform: translateY(0);
  box-shadow: 0 1px 2px rgba(224, 27, 36, 0.1);
}

.logout-button svg {
  flex-shrink: 0;
}

/* 未登录状态 */
.login-prompt {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 8px 16px;
}

.login-prompt.compact {
  padding: 6px 12px;
  gap: 8px;
}

.prompt-text {
  font-size: 14px;
  color: #77767b;
  white-space: nowrap;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .user-nickname {
    max-width: 80px;
  }

  .user-username {
    max-width: 80px;
  }

  .login-prompt:not(.compact) {
    flex-direction: column;
    align-items: flex-start;
    gap: 8px;
  }
}

@media (max-width: 480px) {
  .user-info-container:not(.compact) {
    padding: 6px 12px;
    gap: 12px;
  }

  .user-nickname {
    max-width: 60px;
  }

  .prompt-text {
    display: none;
  }

  .logout-button span {
    display: none;
  }

  .logout-button {
    padding: 8px;
  }

  /* 小屏幕下隐藏性别图标 */
  .gender-icon {
    display: none;
  }
}
</style>
