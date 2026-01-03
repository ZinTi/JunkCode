<!--
输入框控件特性
四种边框状态：
    默认状态：灰色边框
    聚焦状态：蓝色边框，带有发光效果
    验证成功：绿色边框
    验证失败：红色边框
验证反馈：
    验证成功时显示绿色勾选图标
    验证失败时显示红色错误图标和错误信息
灵活的验证机制：
    支持自定义验证函数
    可控制验证时机（输入时或失焦时）
完整的TypeScript支持：
    类型安全的属性定义
    完整的类型推断
响应式设计：
    自适应宽度
    流畅的过渡动画
-->
<template>
  <div class="line-edit-container">
    <!-- 标签部分 -->
    <label
      v-if="label"
      class="line-edit-label"
      :for="inputId"
      :class="{ 'label-error': hasError }"
    >
      {{ label }}
    </label>

    <!-- 输入框容器，包含边框状态 -->
    <div
      class="input-wrapper"
      :class="borderClass"
      @click="focusInput"
    >
      <!-- 输入框 -->
      <input
        :id="inputId"
        ref="inputRef"
        v-model="inputValue"
        :type="inputType"
        class="line-edit-input"
        :placeholder="effectivePlaceholder"
        :disabled="disabled"
        @focus="handleFocus"
        @blur="handleBlur"
        @input="handleInput"
      />

      <!-- 验证成功图标 -->
      <div
        v-if="showSuccessIcon"
        class="success-icon-container"
      >
        <img
          src="../../assets/check-circle-fill-green.svg"
          alt="Valid"
          class="success-icon"
          @error="handleImageError"
        />
      </div>
    </div>

    <!-- 错误信息 -->
    <div
      v-if="showError"
      class="error-message"
    >
      <img
        src="../../assets/alert-fill-12.svg"
        alt="Error"
        class="error-icon"
        @error="handleImageError"
      />
      <span class="error-text">{{ errorMessage }}</span>
    </div>
  </div>
</template>

<script setup lang="ts">
  import {
    ref,
    computed,
    watch,
    onMounted,
    // onUnmounted,
    nextTick
  } from 'vue'

  // 定义组件属性接口
  interface Props {
    // 标签文本
    label?: string
    // 占位符文本，默认为label
    placeholder?: string
    // 双向绑定的值
    modelValue: string
    // 验证函数，返回验证结果和错误信息
    validator?: (value: string) => { isValid: boolean; message?: string }
    // 是否立即验证（输入时验证），默认为true
    validateOnInput?: boolean
    // 是否禁用
    disabled?: boolean
    // 输入框类型
    type?: 'text' | 'password' | 'email' | 'number'
    // 自定义ID
    id?: string
  }

  // 定义组件事件
  interface Emits {
    (e: 'update:modelValue', value: string): void
    (e: 'validity-change', isValid: boolean): void
    (e: 'focus'): void
    (e: 'blur'): void
  }

  // 定义组件属性
  const props = withDefaults(defineProps<Props>(), {
    label: '',
    placeholder: '',
    validateOnInput: true,
    disabled: false,
    type: 'text',
    id: ''
  })

  // 定义组件事件
  const emit = defineEmits<Emits>()

  // 响应式状态
  const inputRef = ref<HTMLInputElement | null>(null)
  const isFocused = ref(false)
  const isTouched = ref(false)
  const validationResult = ref<{ isValid: boolean; message?: string }>({ isValid: true })

  // 生成唯一ID用于label关联
  const inputId = computed(() => props.id || `line-edit-${Math.random().toString(36).substr(2, 9)}`)

  // 计算占位符文本
  const effectivePlaceholder = computed(() => props.placeholder || props.label)

  // 处理双向绑定
  const inputValue = computed({
    get: () => props.modelValue,
    set: (value) => emit('update:modelValue', value)
  })

  // 输入类型处理
  const inputType = computed(() => props.type)

  // 计算边框样式类
  const borderClass = computed(() => {
    if (isFocused.value) {
      return 'border-focused'
    } else if (isTouched.value && hasError.value) {
      return 'border-error'
    } else if (isTouched.value && validationResult.value.isValid && props.modelValue) {
      return 'border-valid'
    } else {
      return 'border-default'
    }
  })

  // 错误状态
  const hasError = computed(() => isTouched.value && !validationResult.value.isValid)
  const showError = computed(() => hasError.value && !!validationResult.value.message)
  const errorMessage = computed(() => validationResult.value.message || '')

  // 成功图标显示条件
  const showSuccessIcon = computed(() =>
    isTouched.value &&
    validationResult.value.isValid &&
    props.modelValue.length > 0
  )

  // 输入框聚焦
  const focusInput = () => {
    if (inputRef.value && !props.disabled) {
      inputRef.value.focus()
    }
  }

  // 处理聚焦事件
  const handleFocus = () => {
    isFocused.value = true
    emit('focus')
  }

  // 处理失焦事件
  const handleBlur = () => {
    isFocused.value = false
    isTouched.value = true
    validate()
    emit('blur')
  }

  // 处理输入事件
  const handleInput = () => {
    if (props.validateOnInput && isTouched.value) {
      validate()
    }
  }

  // 验证函数
  const validate = () => {
    if (!props.validator) {
      // 如果没有提供验证器，则使用默认验证
      validationResult.value = { isValid: true }
    } else {
      validationResult.value = props.validator(props.modelValue)
    }

    // 触发有效性变化事件
    emit('validity-change', validationResult.value.isValid)

    return validationResult.value.isValid
  }

  // 处理图片加载失败
  const handleImageError = (event: Event) => {
    const imgElement = event.target as HTMLImageElement
    console.warn(`无法加载图标: ${imgElement.src}`)

    // 可以设置备用内容
    if (imgElement.src.includes('check-circle-fill-green')) {
      imgElement.outerHTML = '<div class="fallback-success-icon">✓</div>'
    } else if (imgElement.src.includes('alert-fill-12')) {
      imgElement.outerHTML = '<div class="fallback-error-icon">!</div>'
    }
  }

  // 暴露方法给父组件
  defineExpose({
    focus: () => focusInput(),
    validate: () => validate(),
    clear: () => {
      inputValue.value = ''
      isTouched.value = false
      validationResult.value = { isValid: true }
    },
    setTouched: (touched: boolean) => {
      isTouched.value = touched
      if (touched) validate()
    }
  })

  // 监听modelValue变化
  watch(() => props.modelValue, () => {
    if (props.validateOnInput && isTouched.value) {
      validate()
    }
  })

  // 组件挂载时
  onMounted(() => {
    // 如果有初始值，验证它
    if (props.modelValue && props.validator) {
      nextTick(() => {
        isTouched.value = true
        validate()
      })
    }
  })
</script>

<style scoped>
.line-edit-container {
  display: flex;
  flex-direction: column;
  width: 100%;
  max-width: 400px;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
}

.line-edit-label {
  font-size: 14px;
  font-weight: 500;
  color: #333;
  margin-bottom: 6px;
  transition: color 0.2s ease;
}

.label-error {
  color: #d32f2f;
}

.input-wrapper {
  position: relative;
  display: flex;
  align-items: center;
  border: 2px solid;
  border-radius: 8px;
  padding: 0 12px;
  height: 48px;
  cursor: text;
  transition: border-color 0.2s ease, box-shadow 0.2s ease;
}

.border-default {
  border-color: #ccc;
}

.border-focused {
  border-color: #1976d2;
  box-shadow: 0 0 0 2px rgba(25, 118, 210, 0.2);
}

.border-error {
  border-color: #d32f2f;
}

.border-valid {
  border-color: #2e7d32;
}

.line-edit-input {
  flex: 1;
  border: none;
  outline: none;
  font-size: 16px;
  background: transparent;
  color: #333;
  width: 100%;
}

.line-edit-input::placeholder {
  color: #999;
  opacity: 1;
}

.line-edit-input:disabled {
  background-color: #f5f5f5;
  cursor: not-allowed;
  color: #999;
}

.success-icon-container {
  display: flex;
  align-items: center;
  justify-content: center;
  margin-left: 8px;
}

.success-icon {
  width: 20px;
  height: 20px;
  transition: transform 0.2s ease;
}

.success-icon:hover {
  transform: scale(1.1);
}

.error-message {
  display: flex;
  align-items: center;
  margin-top: 6px;
  color: #d32f2f;
  font-size: 13px;
  min-height: 20px;
  animation: fadeIn 0.3s ease;
}

.error-icon {
  width: 16px;
  height: 16px;
  margin-right: 6px;
}

.error-text {
  line-height: 1.4;
}

.fallback-success-icon {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  background-color: #2e7d32;
  color: white;
  font-size: 12px;
  font-weight: bold;
}

.fallback-error-icon {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 16px;
  height: 16px;
  border-radius: 50%;
  background-color: #d32f2f;
  color: white;
  font-size: 10px;
  font-weight: bold;
}

@keyframes fadeIn {
  from {
    opacity: 0;
    transform: translateY(-5px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}
</style>
