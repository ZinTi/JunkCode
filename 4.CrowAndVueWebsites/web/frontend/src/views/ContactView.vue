<template>
  <div class="contact-view">
    <div class="contact-container">
      <!-- 页面标题和说明 -->
      <div class="contact-header">
        <h1 class="page-title">联系我们</h1>
        <p class="page-description">
          有任何问题或建议？请填写下方表单联系我们。我们会尽快给您回复。
        </p>
        <div class="demo-notice">
          <span class="notice-icon">⚠️</span>
          <span>注意：此页面为前端演示，所有提交数据仅保存在浏览器内存中，不会发送到后端服务器。</span>
        </div>
      </div>

      <div class="contact-content">
        <!-- 联系表单 -->
        <div class="form-section">
          <h2 class="section-title">发送消息</h2>
          <form @submit.prevent="handleSubmit" class="contact-form">
            <div class="form-row">
              <div class="form-group">
                <label for="name" class="form-label">姓名 <span class="required">*</span></label>
                <input
                  type="text"
                  id="name"
                  v-model="formData.name"
                  placeholder="请输入您的姓名"
                  class="form-input"
                  :class="{ 'input-error': errors.name }"
                  @blur="validateField('name')"
                  required
                />
                <div v-if="errors.name" class="error-message">{{ errors.name }}</div>
              </div>

              <div class="form-group">
                <label for="email" class="form-label">邮箱 <span class="required">*</span></label>
                <input
                  type="email"
                  id="email"
                  v-model="formData.email"
                  placeholder="example@example.com"
                  class="form-input"
                  :class="{ 'input-error': errors.email }"
                  @blur="validateField('email')"
                  required
                />
                <div v-if="errors.email" class="error-message">{{ errors.email }}</div>
              </div>
            </div>

            <div class="form-group">
              <label for="phone" class="form-label">手机号码</label>
              <input
                type="tel"
                id="phone"
                v-model="formData.phone"
                placeholder="请输入您的手机号码"
                class="form-input"
                :class="{ 'input-error': errors.phone }"
                @blur="validateField('phone')"
              />
              <div v-if="errors.phone" class="error-message">{{ errors.phone }}</div>
            </div>

            <div class="form-group">
              <label for="message" class="form-label">留言内容 <span class="required">*</span></label>
              <textarea
                id="message"
                v-model="formData.message"
                placeholder="请详细描述您的问题或建议..."
                rows="5"
                class="form-textarea"
                :class="{ 'input-error': errors.message }"
                @blur="validateField('message')"
                required
              ></textarea>
              <div class="textarea-info">
                <span>已输入 {{ messageLength }} 个字符</span>
                <span v-if="errors.message" class="error-message">{{ errors.message }}</span>
              </div>
            </div>

            <div class="form-actions">
              <button type="submit" class="submit-btn" :disabled="isSubmitting">
                <span v-if="isSubmitting">提交中...</span>
                <span v-else>提交留言</span>
              </button>
              <button type="button" class="reset-btn" @click="resetForm">重置表单</button>
            </div>
          </form>
        </div>

        <!-- 提交记录展示 -->
        <div class="records-section">
          <div class="section-header">
            <h2 class="section-title">提交记录</h2>
            <div class="records-stats">
              <span class="stats-item">共 {{ submissions.length }} 条记录</span>
              <button
                v-if="submissions.length > 0"
                type="button"
                class="clear-btn"
                @click="clearSubmissions"
              >
                清空记录
              </button>
            </div>
          </div>

          <!-- 空状态 -->
          <div v-if="submissions.length === 0" class="empty-state">
            <div class="empty-icon">📭</div>
            <h3>暂无提交记录</h3>
            <p>提交的表单数据将显示在这里</p>
          </div>

          <!-- 提交记录列表 -->
          <div v-else class="records-list">
            <div
              v-for="(submission, index) in sortedSubmissions"
              :key="submission.id"
              class="record-card"
            >
              <div class="record-header">
                <div class="record-meta">
                  <span class="record-number">#{{ submissions.length - index }}</span>
                  <span class="record-time">{{ formatTime(submission.timestamp) }}</span>
                </div>
                <button
                  type="button"
                  class="delete-record"
                  @click="deleteSubmission(submission.id)"
                  title="删除此记录"
                >
                  ×
                </button>
              </div>

              <div class="record-content">
                <div class="record-field">
                  <span class="field-label">姓名:</span>
                  <span class="field-value">{{ submission.name }}</span>
                </div>
                <div class="record-field">
                  <span class="field-label">邮箱:</span>
                  <span class="field-value">{{ submission.email }}</span>
                </div>
                <div v-if="submission.phone" class="record-field">
                  <span class="field-label">手机:</span>
                  <span class="field-value">{{ submission.phone }}</span>
                </div>
                <div class="record-field message-field">
                  <span class="field-label">留言:</span>
                  <p class="field-value message-content">{{ submission.message }}</p>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- 联系信息 -->
      <div class="contact-info">
        <h2 class="section-title">其他联系方式</h2>
        <div class="info-cards">
          <div class="info-card">
            <div class="info-icon">📧</div>
            <h3>邮箱</h3>
            <p>contact@ccut.edu.cn</p>
            <p>工作日 9:00-17:00 回复</p>
          </div>
          <div class="info-card">
            <div class="info-icon">📞</div>
            <h3>电话</h3>
            <p>+86 (0431) 8571-XXXX</p>
            <p>周一至周五 8:30-11:30, 13:30-16:30</p>
          </div>
          <div class="info-card">
            <div class="info-icon">📍</div>
            <h3>地址</h3>
            <p>吉林省长春市宽城区北湖校区西区</p>
            <p>长春工业大学</p>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive, computed, onMounted } from 'vue'

// 定义表单数据类型
interface FormData {
  name: string
  email: string
  phone: string
  message: string
}

// 定义提交记录类型
interface Submission extends FormData {
  id: number
  timestamp: Date
}

// 定义错误类型
interface FormErrors {
  name?: string
  email?: string
  phone?: string
  message?: string
}

// 响应式数据
const formData = reactive<FormData>({
  name: '',
  email: '',
  phone: '',
  message: ''
})

const errors = reactive<FormErrors>({})
const submissions = ref<Submission[]>([])
const isSubmitting = ref(false)

// 计算属性
const messageLength = computed(() => formData.message.length)
const sortedSubmissions = computed(() =>
  [...submissions.value].sort((a, b) => b.timestamp.getTime() - a.timestamp.getTime())
)

// 验证单个字段
const validateField = (field: keyof FormErrors) => {
  switch (field) {
    case 'name':
      if (!formData.name.trim()) {
        errors.name = '姓名不能为空'
      } else if (formData.name.length < 2) {
        errors.name = '姓名至少需要2个字符'
      } else {
        delete errors.name
      }
      break

    case 'email':
      const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/
      if (!formData.email.trim()) {
        errors.email = '邮箱不能为空'
      } else if (!emailRegex.test(formData.email)) {
        errors.email = '请输入有效的邮箱地址'
      } else {
        delete errors.email
      }
      break

    case 'phone':
      if (formData.phone && !/^1[3-9]\d{9}$/.test(formData.phone)) {
        errors.phone = '请输入有效的手机号码'
      } else {
        delete errors.phone
      }
      break

    case 'message':
      if (!formData.message.trim()) {
        errors.message = '留言内容不能为空'
      } else if (formData.message.length < 10) {
        errors.message = '留言内容至少需要10个字符'
      } else if (formData.message.length > 500) {
        errors.message = '留言内容不能超过500个字符'
      } else {
        delete errors.message
      }
      break
  }
}

// 验证整个表单
const validateForm = (): boolean => {
  validateField('name')
  validateField('email')
  validateField('phone')
  validateField('message')

  return Object.keys(errors).length === 0
}

// 处理表单提交
const handleSubmit = () => {
  if (!validateForm()) {
    alert('请正确填写表单中的所有必填项')
    return
  }

  isSubmitting.value = true

  // 模拟网络请求延迟
  setTimeout(() => {
    // 创建提交记录
    const newSubmission: Submission = {
      id: Date.now(),
      timestamp: new Date(),
      ...formData
    }

    // 添加到提交记录列表
    submissions.value.unshift(newSubmission)

    // 保存到本地存储（用于演示数据持久化）
    saveToLocalStorage()

    // 重置表单
    resetForm()

    // 显示成功消息
    alert('提交成功！您的留言已保存到本地。')

    isSubmitting.value = false
  }, 800)
}

// 重置表单
const resetForm = () => {
  formData.name = ''
  formData.email = ''
  formData.phone = ''
  formData.message = ''

  // 清除所有错误
  Object.keys(errors).forEach(key => {
    delete errors[key as keyof FormErrors]
  })
}

// 删除单条提交记录
const deleteSubmission = (id: number) => {
  if (confirm('确定要删除这条记录吗？')) {
    submissions.value = submissions.value.filter(submission => submission.id !== id)
    saveToLocalStorage()
  }
}

// 清空所有提交记录
const clearSubmissions = () => {
  if (submissions.value.length === 0) return

  if (confirm('确定要清空所有提交记录吗？此操作不可撤销。')) {
    submissions.value = []
    localStorage.removeItem('contact_submissions')
  }
}

// 格式化时间显示
const formatTime = (date: Date): string => {
  const now = new Date()
  const diffMs = now.getTime() - date.getTime()
  const diffMins = Math.floor(diffMs / (1000 * 60))
  const diffHours = Math.floor(diffMs / (1000 * 60 * 60))
  const diffDays = Math.floor(diffMs / (1000 * 60 * 60 * 24))

  if (diffMins < 1) return '刚刚'
  if (diffMins < 60) return `${diffMins}分钟前`
  if (diffHours < 24) return `${diffHours}小时前`
  if (diffDays < 7) return `${diffDays}天前`

  return date.toLocaleDateString('zh-CN', {
    month: 'short',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit'
  })
}

// 保存到本地存储
const saveToLocalStorage = () => {
  try {
    localStorage.setItem('contact_submissions', JSON.stringify(submissions.value))
  } catch (error) {
    console.error('保存到本地存储失败:', error)
  }
}

// 从本地存储加载数据
const loadFromLocalStorage = () => {
  try {
    const stored = localStorage.getItem('contact_submissions')
    if (stored) {
      const parsed = JSON.parse(stored)
      // 恢复Date对象
      submissions.value = parsed.map((item: any) => ({
        ...item,
        timestamp: new Date(item.timestamp)
      }))
    }
  } catch (error) {
    console.error('从本地存储加载数据失败:', error)
  }
}

// 组件挂载时加载数据
onMounted(() => {
  loadFromLocalStorage()
})
</script>

<style scoped>
.contact-view {
  min-height: 100vh;
  background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
  padding: 2rem 1rem;
}

.contact-container {
  max-width: 1200px;
  margin: 0 auto;
}

.contact-header {
  text-align: center;
  margin-bottom: 3rem;
}

.page-title {
  font-size: 2.8rem;
  font-weight: 700;
  color: #2c3e50;
  margin-bottom: 1rem;
  background: linear-gradient(90deg, #3498db, #2c3e50);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.page-description {
  font-size: 1.1rem;
  color: #5a6c7d;
  max-width: 700px;
  margin: 0 auto 1.5rem;
  line-height: 1.6;
}

.demo-notice {
  display: inline-flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.8rem 1.5rem;
  background: linear-gradient(135deg, #ffecd2 0%, #fcb69f 100%);
  border-radius: 12px;
  border-left: 4px solid #e74c3c;
  margin-top: 1rem;
}

.notice-icon {
  font-size: 1.2rem;
}

.contact-content {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 2.5rem;
  margin-bottom: 4rem;
}

@media (max-width: 992px) {
  .contact-content {
    grid-template-columns: 1fr;
  }
}

.section-title {
  font-size: 1.8rem;
  font-weight: 600;
  color: #2c3e50;
  margin-bottom: 1.5rem;
  padding-bottom: 0.5rem;
  border-bottom: 2px solid #3498db;
}

.form-section,
.records-section {
  background: white;
  border-radius: 16px;
  padding: 2rem;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.08);
  transition: transform 0.3s ease, box-shadow 0.3s ease;
}

.form-section:hover,
.records-section:hover {
  transform: translateY(-5px);
  box-shadow: 0 15px 35px rgba(0, 0, 0, 0.12);
}

.form-row {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 1.5rem;
  margin-bottom: 1.5rem;
}

@media (max-width: 576px) {
  .form-row {
    grid-template-columns: 1fr;
  }
}

.form-group {
  margin-bottom: 0.5rem;
}

.form-label {
  display: block;
  font-weight: 600;
  margin-bottom: 0.5rem;
  color: #2c3e50;
}

.required {
  color: #e74c3c;
}

.form-input,
.form-textarea {
  width: 80%;
  padding: 0.9rem 1.2rem;
  border: 2px solid #e0e6ed;
  border-radius: 10px;
  font-size: 1rem;
  transition: all 0.3s ease;
  background-color: #f8fafc;
}

.form-input:focus,
.form-textarea:focus {
  outline: none;
  border-color: #3498db;
  background-color: white;
  box-shadow: 0 0 0 3px rgba(52, 152, 219, 0.2);
}

.input-error {
  border-color: #e74c3c !important;
}

.error-message {
  color: #e74c3c;
  font-size: 0.85rem;
  margin-top: 0.4rem;
}

.textarea-info {
  display: flex;
  justify-content: space-between;
  margin-top: 0.4rem;
  font-size: 0.85rem;
  color: #7f8c8d;
}

.form-actions {
  display: flex;
  gap: 1rem;
  margin-top: 2rem;
}

.submit-btn,
.reset-btn,
.clear-btn {
  padding: 0.9rem 2rem;
  border-radius: 10px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
  border: none;
}

.submit-btn {
  background: linear-gradient(135deg, #3498db, #2980b9);
  color: white;
  flex: 1;
}

.submit-btn:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 5px 15px rgba(52, 152, 219, 0.4);
}

.submit-btn:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

.reset-btn {
  background: white;
  color: #7f8c8d;
  border: 2px solid #e0e6ed;
}

.reset-btn:hover {
  background: #f8f9fa;
  border-color: #bdc3c7;
}

.section-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 1.5rem;
}

.records-stats {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.stats-item {
  font-size: 0.9rem;
  color: #7f8c8d;
}

.clear-btn {
  padding: 0.5rem 1rem;
  background: #f8f9fa;
  color: #e74c3c;
  border: 1px solid #e74c3c;
  font-size: 0.9rem;
  border-radius: 6px;
}

.clear-btn:hover {
  background: #e74c3c;
  color: white;
}

.empty-state {
  text-align: center;
  padding: 3rem 2rem;
  color: #95a5a6;
}

.empty-icon {
  font-size: 4rem;
  margin-bottom: 1rem;
}

.records-list {
  max-height: 600px;
  overflow-y: auto;
  padding-right: 0.5rem;
}

.record-card {
  background: #f8f9fa;
  border-radius: 12px;
  padding: 1.5rem;
  margin-bottom: 1rem;
  border-left: 4px solid #3498db;
  transition: transform 0.2s ease;
}

.record-card:hover {
  transform: translateX(5px);
}

.record-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 1rem;
}

.record-meta {
  display: flex;
  gap: 1rem;
  align-items: center;
}

.record-number {
  font-weight: 700;
  color: #3498db;
  background: rgba(52, 152, 219, 0.1);
  padding: 0.2rem 0.6rem;
  border-radius: 20px;
  font-size: 0.9rem;
}

.record-time {
  font-size: 0.85rem;
  color: #7f8c8d;
}

.delete-record {
  background: none;
  border: none;
  font-size: 1.5rem;
  color: #95a5a6;
  cursor: pointer;
  width: 30px;
  height: 30px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 50%;
  transition: all 0.2s ease;
}

.delete-record:hover {
  background: #e74c3c;
  color: white;
}

.record-field {
  display: flex;
  margin-bottom: 0.8rem;
}

.field-label {
  font-weight: 600;
  min-width: 60px;
  color: #2c3e50;
}

.field-value {
  color: #34495e;
  flex: 1;
}

.message-field {
  flex-direction: column;
}

.message-content {
  background: white;
  padding: 1rem;
  border-radius: 8px;
  margin-top: 0.5rem;
  line-height: 1.5;
}

.contact-info {
  background: white;
  border-radius: 16px;
  padding: 2.5rem;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.08);
}

.info-cards {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 2rem;
  margin-top: 2rem;
}

.info-card {
  text-align: center;
  padding: 2rem 1.5rem;
  background: linear-gradient(135deg, #f8f9fa 0%, #e9ecef 100%);
  border-radius: 12px;
  transition: transform 0.3s ease;
}

.info-card:hover {
  transform: translateY(-8px);
}

.info-icon {
  font-size: 3rem;
  margin-bottom: 1rem;
}

.info-card h3 {
  font-size: 1.4rem;
  margin-bottom: 1rem;
  color: #2c3e50;
}

.info-card p {
  color: #5a6c7d;
  margin-bottom: 0.5rem;
  line-height: 1.5;
}

/* 自定义滚动条 */
.records-list::-webkit-scrollbar {
  width: 8px;
}

.records-list::-webkit-scrollbar-track {
  background: #f1f1f1;
  border-radius: 4px;
}

.records-list::-webkit-scrollbar-thumb {
  background: #bdc3c7;
  border-radius: 4px;
}

.records-list::-webkit-scrollbar-thumb:hover {
  background: #95a5a6;
}
</style>
