<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'

// 定义雨滴对象类型
interface RainDrop {
  x: number
  y: number
  speed: number
  length: number
  chars: string[]
  opacity: number
}

// 定义Canvas相关引用
const canvasRef = ref<HTMLCanvasElement | null>(null)
const animationFrameId = ref<number | null>(null)
const drops = ref<RainDrop[]>([])
const fps = ref(0) // 添加帧率显示
const lastTime = ref(performance.now())
const frameCount = ref(0)
const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$+-*/=%"\'#&_(),.;:?!\\|{}<>[]^~'
const fontSize = 16
let columns = 0
let rows = 0

// 计算帧率
const calculateFPS = (currentTime: number) => {
  frameCount.value++

  if (currentTime - lastTime.value >= 1000) {
    fps.value = Math.round((frameCount.value * 1000) / (currentTime - lastTime.value))
    frameCount.value = 0
    lastTime.value = currentTime
  }
}

// 初始化代码雨
const initCodeRain = () => {
  if (!canvasRef.value) return

  const canvas = canvasRef.value
  const ctx = canvas.getContext('2d')
  if (!ctx) return

  // 设置Canvas大小
  canvas.width = window.innerWidth
  canvas.height = window.innerHeight

  // 计算列数和行数
  columns = Math.floor(canvas.width / fontSize)
  rows = Math.floor(canvas.height / fontSize)

  // 清空之前的雨滴
  drops.value = []

  // 创建初始雨滴
  for (let i = 0; i < columns; i++) {
    const length = Math.floor(Math.random() * 10) + 5
    const drop: RainDrop = {
      x: i * fontSize,
      y: Math.random() * -canvas.height,
      // 进一步减慢速度：从 Math.random() * 2 + 1 改为 Math.random() * 1.5 + 0.5
      speed: Math.random() * 1.5 + 0.5,
      length: length,
      chars: Array.from({ length }, () => chars[Math.floor(Math.random() * chars.length)] as string),
      opacity: Math.random() * 0.5 + 0.5
    }
    drops.value.push(drop)
  }
}

// 绘制代码雨
const drawCodeRain = () => {
  if (!canvasRef.value) return

  const canvas = canvasRef.value
  const ctx = canvas.getContext('2d')
  if (!ctx) return

  // 计算帧率
  calculateFPS(performance.now())

  // 绘制渐变背景
  const gradient = ctx.createLinearGradient(0, 0, 0, canvas.height)
  gradient.addColorStop(0, '#1e3c72')
  gradient.addColorStop(1, '#2a5298')

  // 半透明黑色背景用于拖尾效果
  ctx.fillStyle = 'rgba(0, 0, 0, 0.05)'
  ctx.fillRect(0, 0, canvas.width, canvas.height)

  // 绘制每个雨滴
  ctx.font = `bold ${fontSize}px 'Courier New', monospace`

  drops.value.forEach((drop, index) => {
    // 更新雨滴位置
    drop.y += drop.speed

    // 如果雨滴超出屏幕，重置到顶部
    if (drop.y > canvas.height + drop.length * fontSize) {
      drop.y = -drop.length * fontSize
      // 进一步减慢速度：从 Math.random() * 2 + 1 改为 Math.random() * 1.5 + 0.5
      drop.speed = Math.random() * 1.5 + 0.5
      drop.opacity = Math.random() * 0.5 + 0.5

      // 随机更新一些字符
      drop.chars = drop.chars.map((_, i) =>
        Math.random() > 0.9 ? chars[Math.floor(Math.random() * chars.length)]! : drop.chars[i]!
      )
    }

    // 绘制雨滴的每个字符
    drop.chars.forEach((char, i) => {
      const yPos = drop.y - i * fontSize

      // 只绘制在屏幕内的字符
      if (yPos < 0 || yPos > canvas.height) return

      // 计算字符透明度（第一个字符最亮）
      const charOpacity = drop.opacity * (1 - i / drop.length)

      // 绘制字符阴影
      ctx.fillStyle = `rgba(135, 206, 250, ${charOpacity * 0.3})`
      ctx.fillText(char, drop.x + 1, yPos + 1)

      // 绘制字符
      if (i === 0) {
        // 第一个字符为白色
        ctx.fillStyle = `rgba(255, 255, 255, ${charOpacity})`
      } else if (i < 3) {
        // 前三个字符为浅蓝色
        ctx.fillStyle = `rgba(173, 216, 230, ${charOpacity})`
      } else {
        // 其余字符为蓝色
        ctx.fillStyle = `rgba(0, 100, 255, ${charOpacity})`
      }

      ctx.fillText(char, drop.x, yPos)
    })
  })

  // 继续动画循环
  animationFrameId.value = requestAnimationFrame(drawCodeRain)
}

// 处理窗口大小变化
const handleResize = () => {
  if (animationFrameId.value) {
    cancelAnimationFrame(animationFrameId.value)
  }
  initCodeRain()
  animationFrameId.value = requestAnimationFrame(drawCodeRain)
}

// 生命周期钩子
onMounted(() => {
  initCodeRain()
  animationFrameId.value = requestAnimationFrame(drawCodeRain)
  window.addEventListener('resize', handleResize)
})

onUnmounted(() => {
  if (animationFrameId.value) {
    cancelAnimationFrame(animationFrameId.value)
  }
  window.removeEventListener('resize', handleResize)
})
</script>

<template>
  <main class="code-rain-container">
    <canvas ref="canvasRef" class="code-rain-canvas"></canvas>

    <div class="content">
      <div class="info-card floating">
        <h2>首页</h2>
        <ul>
          <li>Vue 3 组合式API</li>
          <li>TypeScript 类型安全</li>
          <li>Canvas 动画渲染</li>
          <li>响应式设计</li>
        </ul>

        <div class="features">
          <div class="feature">
            <span class="feature-label">字符数量：</span>
            <span class="feature-value">{{ drops.length }}</span>
          </div>
          <div class="feature">
            <span class="feature-label">帧率：</span>
            <span class="feature-value">{{ fps }} FPS</span>
          </div>
        </div>
      </div>
    </div>
  </main>
</template>

<style scoped>
.code-rain-container {
  position: relative;
  width: 100%;
  height: 100%;
  overflow: hidden;
  background: linear-gradient(180deg, #1e3c72 0%, #2a5298 100%);
}

.code-rain-canvas {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  display: block;
}

.content {
  position: relative;
  z-index: 10;
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 2rem;
  text-align: center;
  color: white;
  text-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
}

@keyframes glow {
  from {
    text-shadow: 0 0 10px rgba(135, 206, 235, 0.2),
                 0 0 20px rgba(135, 206, 235, 0.1);
  }
  to {
    text-shadow: 0 0 20px rgba(135, 206, 235, 0.4),
                 0 0 30px rgba(135, 206, 235, 0.3),
                 0 0 40px rgba(135, 206, 235, 0.2);
  }
}

/* 漂浮动画效果 */
@keyframes float {
  0%, 100% {
    transform: translateY(0) translateX(0) rotate(0deg);
  }
  20% {
    transform: translateY(-25px) translateX(12px) rotate(3deg);
  }
  40% {
    transform: translateY(-15px) translateX(-10px) rotate(-2.5deg);
  }
  60% {
    transform: translateY(-30px) translateX(8px) rotate(2deg);
  }
  80% {
    transform: translateY(-20px) translateX(-6px) rotate(-2deg);
  }
}

.info-card {
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(10px);
  border-radius: 16px;
  padding: 2rem;
  max-width: 500px;
  width: 100%;
  margin-bottom: 2rem;
  border: 1px solid rgba(255, 255, 255, 0.2);
  box-shadow:
    0 8px 32px rgba(0, 0, 0, 0.2),
    0 0 20px rgba(135, 206, 235, 0.1);
  transition: all 0.3s ease;
}

.info-card:hover {
  transform: scale(1.02);
  box-shadow:
    0 12px 40px rgba(0, 0, 0, 0.3),
    0 0 30px rgba(135, 206, 235, 0.2);
}

/* 应用漂浮动画 */
.info-card.floating {
  animation: float 6s ease-in-out infinite;
  animation-delay: 0.5s;
}

.info-card h2 {
  font-size: 2.5rem;
  margin: 0;
  color: #87ceeb;
  text-align: center;
  background: linear-gradient(45deg, #87ceeb, #ffffff);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  animation: glow 2s ease-in-out infinite alternate;
}

.info-card ul {
  list-style: none;
  padding: 0;
  margin-bottom: 2rem;
  text-align: left;
}

.info-card li {
  padding: 0.75rem 0;
  font-size: 1.2rem;
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
  display: flex;
  align-items: center;
  transition: all 0.3s ease;
}

.info-card li:hover {
  transform: translateX(5px);
  background: rgba(255, 255, 255, 0.05);
  border-radius: 4px;
  padding-left: 10px;
}

.info-card li:before {
  content: "▸";
  color: #87ceeb;
  margin-right: 1rem;
  font-size: 1.5rem;
  transition: all 0.3s ease;
}

.info-card li:hover:before {
  transform: scale(1.2);
  color: #ffffff;
}

.features {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 1rem;
  margin-top: 1.5rem;
}

.feature {
  background: rgba(0, 0, 0, 0.2);
  padding: 1rem;
  border-radius: 8px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  transition: all 0.3s ease;
  border: 1px solid rgba(135, 206, 235, 0.1);
}

.feature:hover {
  background: rgba(0, 0, 0, 0.3);
  border-color: rgba(135, 206, 235, 0.3);
  transform: translateY(-2px);
}

.feature-label {
  color: rgba(255, 255, 255, 0.8);
  font-size: 0.9rem;
}

.feature-value {
  color: #87ceeb;
  font-weight: bold;
  font-family: 'Courier New', monospace;
  font-size: 1.1rem;
  text-shadow: 0 0 5px rgba(135, 206, 235, 0.5);
}

/* 响应式设计 */
@media (max-width: 768px) {
  .info-card {
    padding: 1.5rem;
    margin: 0 1rem;
  }

  .info-card.floating {
    animation: float 10s ease-in-out infinite;
  }

  .features {
    grid-template-columns: 1fr;
  }

  .info-card h2 {
    font-size: 2rem;
  }
}

@media (max-width: 480px) {
  .content {
    padding: 1rem;
  }

  .info-card {
    padding: 1.2rem;
  }

  .info-card.floating {
    animation: float 12s ease-in-out infinite;
  }

  .info-card h2 {
    font-size: 1.8rem;
  }

  .info-card li {
    font-size: 1rem;
  }

  .feature-label {
    font-size: 0.8rem;
  }

  .feature-value {
    font-size: 1rem;
  }
}
</style>
