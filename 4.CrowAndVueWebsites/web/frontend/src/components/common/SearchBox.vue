<script lang="ts" setup>
import { ref, onMounted } from 'vue'

// 定义组件接收的属性
interface Props {
  placeholder?: string
  size?: 'small' | 'medium' | 'large'
  rounded?: boolean
  showIcon?: boolean
  autoFocus?: boolean
}

const props = withDefaults(defineProps<Props>(), {
  placeholder: '请输入搜索内容...',
  size: 'medium',
  rounded: true,
  showIcon: true,
  autoFocus: false
})

// 定义发出的事件
const emit = defineEmits<{
  search: [value: string]
}>()

// 响应式数据
const searchText = ref('')
const showModal = ref(false)
const modalContent = ref('')
const isFocused = ref(false)
const searchInput = ref<HTMLInputElement | null>(null)

// 搜索方法
const handleSearch = () => {
  if (searchText.value.trim()) {
    modalContent.value = `你搜索了: ${searchText.value}`
    showModal.value = true

    // 向父组件发射搜索事件
    emit('search', searchText.value)
  }
}

// 处理键盘事件
const handleKeyPress = (event: KeyboardEvent) => {
  if (event.key === 'Enter') {
    handleSearch()
  }
}

// 关闭弹窗
const closeModal = () => {
  showModal.value = false
}

// 清空搜索框
const clearSearch = () => {
  searchText.value = ''
  if (searchInput.value) {
    searchInput.value.focus()
  }
}

// 自动聚焦
onMounted(() => {
  if (props.autoFocus && searchInput.value) {
    searchInput.value.focus()
  }
})
</script>

<template>
  <div class="search-container">
    <div
      class="search-box"
      :class="[
        size,
        { rounded, focused: isFocused }
      ]"
    >
      <input
        ref="searchInput"
        v-model="searchText"
        type="text"
        :placeholder="placeholder"
        @keypress="handleKeyPress"
        @focus="isFocused = true"
        @blur="isFocused = false"
        class="search-input"
      />

      <div class="clear-icon" v-if="searchText" @click="clearSearch">
        <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <line x1="18" y1="6" x2="6" y2="18"></line>
          <line x1="6" y1="6" x2="18" y2="18"></line>
        </svg>
      </div>

      <!-- 修改为矩形搜索按钮 -->
      <button
        class="search-button"
        @click="handleSearch"
        :disabled="!searchText.trim()"
        :title="searchText.trim() ? '搜索' : '请输入搜索内容'"
      >
        <svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
          <circle cx="11" cy="11" r="8"></circle>
          <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
        </svg>
        <span class="button-text">搜索</span>
      </button>
    </div>

    <!-- 搜索结果弹窗 -->
    <div v-if="showModal" class="modal-overlay" @click="closeModal">
      <div class="modal-content" @click.stop>
        <div class="modal-header">
          <h3>搜索结果</h3>
          <button class="modal-close" @click="closeModal">×</button>
        </div>
        <div class="modal-body">
          <p>{{ modalContent }}</p>
        </div>
        <div class="modal-footer">
          <button class="modal-ok-button" @click="closeModal">确定</button>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.search-container {
  font-family: 'Inter', 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  display: flex;
  justify-content: center;
  align-items: center;
  width: 100%;
}

.search-box {
  display: flex;
  align-items: center;
  background-color: #f6f5f4; /* GNOME风格的浅灰色背景 */
  border: 2px solid transparent;
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  box-shadow:
    0 1px 3px rgba(0, 0, 0, 0.05),
    inset 0 1px 0 rgba(255, 255, 255, 0.7);
  overflow: hidden;
  position: relative;
}

.search-box.focused {
  background-color: #ffffff;
  border-color: #3584e4; /* GNOME风格的蓝色 */
  box-shadow:
    0 2px 8px rgba(53, 132, 228, 0.2),
    inset 0 1px 0 rgba(255, 255, 255, 0.9);
}

.search-box.rounded {
  border-radius: 12px; /* 调整圆角大小 */
}

.search-box.small {
  width: 300px;
  height: 44px;
}

.search-box.medium {
  width: 500px;
  height: 54px;
}

.search-box.large {
  width: 700px;
  height: 64px;
}

.search-input {
  flex: 1;
  height: 100%;
  border: none;
  outline: none;
  padding: 0 20px;
  font-size: 16px;
  color: #241f31; /* GNOME风格的深色文字 */
  background: transparent;
  font-weight: 400;
  letter-spacing: 0.01em;
}

.search-input::placeholder {
  color: #77767b; /* GNOME风格的占位符颜色 */
  font-weight: 400;
  opacity: 0.8;
}

.clear-icon {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 36px;
  height: 36px;
  margin-right: 8px;
  color: #8b8a8f; /* GNOME风格的图标颜色 */
  cursor: pointer;
  transition: all 0.2s ease;
  border-radius: 8px; /* 添加圆角 */
}

.clear-icon:hover {
  color: #1a1a1a;
  background-color: rgba(0, 0, 0, 0.05);
}

/* 修改为GNOME风格矩形搜索按钮 */
.search-button {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  height: calc(100% - 12px); /* 高度略小于搜索框，留出边距 */
  margin: 6px 8px 6px 0; /* 上下边距，与搜索框对齐 */
  padding: 0 20px;
  background-color: #3584e4; /* GNOME风格的蓝色 */
  color: white;
  border: none;
  border-radius: 8px; /* 矩形圆角 */
  cursor: pointer;
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  font-size: 15px;
  font-weight: 500;
  white-space: nowrap;
  box-shadow:
    0 2px 4px rgba(53, 132, 228, 0.3);
}

.search-button:hover:not(:disabled) {
  background-color: #1c71d8; /* 稍深的蓝色 */
  transform: translateY(-1px);
  box-shadow:
    0 4px 8px rgba(53, 132, 228, 0.4);
}

.search-button:active:not(:disabled) {
  transform: translateY(0);
  box-shadow:
    0 1px 2px rgba(53, 132, 228, 0.4);
}

.search-button:disabled {
  background-color: #c0bfc6; /* GNOME风格的禁用颜色 */
  cursor: not-allowed;
  box-shadow: none;
}

.search-button svg {
  stroke-width: 2.5;
  width: 20px;
  height: 20px;
}

.button-text {
  font-size: 15px;
  font-weight: 500;
  letter-spacing: 0.01em;
}

/* 弹窗样式 - 更新为GNOME风格 */
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: rgba(0, 0, 0, 0.3);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
  animation: fadeIn 0.3s ease;
}

.modal-content {
  background-color: #fafafa; /* GNOME风格的浅色背景 */
  border-radius: 16px; /* 更大的圆角 */
  box-shadow:
    0 10px 30px rgba(0, 0, 0, 0.15),
    0 1px 3px rgba(0, 0, 0, 0.1);
  width: 420px;
  max-width: 90%;
  overflow: hidden;
  animation: slideIn 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  border: 1px solid rgba(0, 0, 0, 0.05);
}

.modal-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 22px 26px;
  background-color: #3584e4; /* GNOME风格的蓝色 */
  color: white;
}

.modal-header h3 {
  margin: 0;
  font-size: 20px;
  font-weight: 600;
  letter-spacing: -0.01em;
}

.modal-close {
  background: transparent;
  border: none;
  color: white;
  font-size: 28px;
  cursor: pointer;
  line-height: 1;
  padding: 0;
  width: 32px;
  height: 32px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 50%;
  transition: background-color 0.2s;
}

.modal-close:hover {
  background-color: rgba(255, 255, 255, 0.2);
}

.modal-body {
  padding: 32px 26px;
  font-size: 18px;
  color: #241f31; /* GNOME风格的深色文字 */
  text-align: center;
  line-height: 1.5;
}

.modal-footer {
  padding: 18px 26px;
  text-align: right;
  background-color: #f0f0f0;
  border-top: 1px solid rgba(0, 0, 0, 0.05);
}

.modal-ok-button {
  padding: 10px 26px;
  background-color: #3584e4;
  color: white;
  border: none;
  border-radius: 10px; /* 更大的圆角 */
  font-size: 16px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s;
  box-shadow: 0 2px 4px rgba(53, 132, 228, 0.3);
}

.modal-ok-button:hover {
  background-color: #1c71d8;
  transform: translateY(-1px);
  box-shadow: 0 4px 8px rgba(53, 132, 228, 0.4);
}

.modal-ok-button:active {
  transform: translateY(0);
  box-shadow: 0 1px 2px rgba(53, 132, 228, 0.4);
}

/* 动画效果 */
@keyframes fadeIn {
  from { opacity: 0; }
  to { opacity: 1; }
}

@keyframes slideIn {
  from {
    opacity: 0;
    transform: translateY(-20px) scale(0.98);
  }
  to {
    opacity: 1;
    transform: translateY(0) scale(1);
  }
}

/* 响应式设计 */
@media (max-width: 768px) {
  .search-box.medium, .search-box.large {
    width: 90%;
  }

  .search-box.small {
    width: 85%;
  }

  .modal-content {
    width: 90%;
  }

  /* 移动端隐藏按钮文字，只显示图标 */
  .search-button {
    padding: 0 12px;
  }

  .button-text {
    display: none;
  }

  .search-button svg {
    margin: 0;
  }
}

@media (max-width: 480px) {
  .search-box.small,
  .search-box.medium,
  .search-box.large {
    height: 52px;
    width: 95%;
  }

  .search-input {
    font-size: 16px;
    padding: 0 16px;
  }

  .search-button {
    height: calc(100% - 10px);
    margin: 5px 6px 5px 0;
    padding: 0 10px;
  }

  .clear-icon {
    width: 32px;
    height: 32px;
    margin-right: 6px;
  }
}

/* 添加一些GNOME风格的微调 */
.search-box {
  backdrop-filter: blur(10px);
}

.search-input:focus {
  caret-color: #3584e4; /* GNOME风格的蓝色光标 */
}

/* 为搜索按钮添加平滑过渡效果 */
.search-button:not(:disabled) {
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
}
</style>
