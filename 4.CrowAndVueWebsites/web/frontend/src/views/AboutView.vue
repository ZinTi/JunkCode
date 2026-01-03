<template>
<WaterMark
  :text="watermark_text"
  :fontSize="18"
  fontColor="#0963FF"
  :opacity="0.50"
  :rotate="-30"
><!-- 使用水印组件 -->
  <div class="about-view">
    <div class="header-content">
      <h1>关于</h1>
      <p>Web前端开发技术：HTML, CSS, JS, TS, Vue3, etc.</p>
    </div>

    <div class="table-container">
      <table class="tech-table">
        <thead>
          <tr>
            <th>模块</th>
            <th>技术栈</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>后端</td>
            <td>C++, Crow 框架</td>
          </tr>
          <tr>
            <td>数据库</td>
            <td>SQLite3</td>
          </tr>
          <tr>
            <td>前端</td>
            <td>Vue3, TypeScript</td>
          </tr>
          <tr>
            <td>API风格</td>
            <td>RESTful</td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</WaterMark>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { useRoute } from 'vue-router'
import WaterMark from '../components/common/WaterMark.vue'

// 获取当前路由对象
const route = useRoute()

// 水印文字
const watermark_text = computed(() => route.meta.watermark_text as string)


</script>

<style scoped>
.about-view {
  min-height: 100vh;
  padding: 1rem;
  display: flex;
  flex-direction: column;
  align-items: flex-start; /* 改为左对齐 */
  position: relative;
}

.header-content {
  width: 100%;
  text-align: left; /* 标题和文本左对齐 */
  margin-bottom: 2rem;
}

.about-view h1 {
  margin-bottom: 1rem;
  color: #2c3e50;
  font-size: 2.5rem;
}

.about-view p {
  margin-bottom: 0;
  color: #666;
  font-size: 1.2rem;
}

.table-container {
  width: 100%;
  display: flex;
  justify-content: center; /* 表格水平居中 */
  align-items: flex-start; /* 表格垂直靠顶 */
  margin-top: 1rem; /* 与标题内容保持一定间距 */
}

.tech-table {
  width: 100%;
  max-width: 600px;
  border-collapse: collapse;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
  border-radius: 8px;
  overflow: hidden;
}

.tech-table thead {
  background: linear-gradient(180deg, #1e3c72 0%, #2a5298 100%); /* 使用渐变色 */
  color: white;
}

.tech-table th,
.tech-table td {
  padding: 1rem 1.5rem;
  text-align: left;
  border-bottom: 1px solid #e0e0e0;
}

.tech-table th {
  font-weight: 600;
  font-size: 1.1rem;
  letter-spacing: 0.5px;
}

.tech-table tbody tr {
  transition: background-color 0.2s;
}

.tech-table tbody tr:hover {
  background-color: #f8f9fa;
}

.tech-table tbody tr:last-child td {
  border-bottom: none;
}

.tech-table td:first-child {
  font-weight: 600;
  color: #2c3e50;
  width: 120px;
}

.tech-table td:last-child {
  color: #666;
}

@media (min-width: 1024px) {
  .about {
    padding: 3rem;
  }

  .tech-table {
    width: 80%;
  }

  .table-container {
    margin-top: 2rem;
  }
}

@media (max-width: 768px) {
  .about {
    padding: 1.5rem;
  }

  .about h1 {
    font-size: 2rem;
  }

  .tech-table th,
  .tech-table td {
    padding: 0.75rem 1rem;
  }

  .table-container {
    margin-top: 1.5rem;
  }
}

@media (max-width: 480px) {
  .about {
    padding: 1rem;
  }

  .about h1 {
    font-size: 1.8rem;
  }

  .about p {
    font-size: 1.1rem;
  }

  .tech-table {
    font-size: 0.9rem;
  }
}
</style>
