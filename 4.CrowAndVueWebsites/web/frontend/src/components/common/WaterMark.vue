<template>
  <div class="watermark-wrapper" :style="wrapperStyle">
    <div class="content-slot">
      <slot></slot>
    </div>
    <div class="watermark-overlay" :style="watermarkStyle"></div>
  </div>
</template>

<script lang="ts">
import { defineComponent, computed, onMounted, onUnmounted, type CSSProperties } from 'vue';

export default defineComponent({
  name: 'WaterMark',
  props: {
    text: {
      type: String,
      default: '水印文本示例'
    },
    fontSize: {
      type: Number,
      default: 20
    },
    fontColor: {
      type: String,
      default: '#000000'
    },
    opacity: {
      type: Number,
      default: 0.1,
      validator: (value: number) => {
        return value >= 0 && value <= 1;
      }
    },
    rotate: {
      type: Number,
      default: -30
    }
  },
  setup(props) {
    // 生成水印背景图片
    const generateWatermark = (): string => {
      const canvas = document.createElement('canvas');
      const ctx = canvas.getContext('2d');

      if (!ctx) return '';

      // 根据文字长度设置canvas大小
      const fontSize = props.fontSize;
      const text = props.text;
      const rotateAngle = props.rotate;

      // 计算文字宽度
      ctx.font = `${fontSize}px Arial`;
      const textWidth = ctx.measureText(text).width;

      // 设置canvas大小，留出足够的空间
      const canvasWidth = Math.max(textWidth + 100, 300);
      const canvasHeight = Math.max(fontSize * 3, 200);
      canvas.width = canvasWidth;
      canvas.height = canvasHeight;

      // 清空画布
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      // 设置文字样式
      ctx.font = `${fontSize}px Arial`;
      ctx.fillStyle = props.fontColor;
      ctx.globalAlpha = props.opacity;
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';

      // 移动到中心点
      const centerX = canvasWidth / 2;
      const centerY = canvasHeight / 2;
      ctx.translate(centerX, centerY);

      // 旋转
      ctx.rotate((rotateAngle * Math.PI) / 180);

      // 绘制文字
      ctx.fillText(text, 0, 0);

      // 恢复旋转和移动
      ctx.rotate((-rotateAngle * Math.PI) / 180);
      ctx.translate(-centerX, -centerY);

      // 转换为base64
      return canvas.toDataURL('image/png');
    };

    // 水印样式 - 修复类型问题
    const watermarkStyle = computed<CSSProperties>(() => {
      const watermarkUrl = generateWatermark();
      return {
        backgroundImage: `url(${watermarkUrl})`,
        backgroundRepeat: 'repeat' as const,
        pointerEvents: 'none' as const,
        zIndex: 9999,
        position: 'absolute' as const,
        top: '0',
        left: '0',
        width: '100%',
        height: '100%'
      };
    });

    // 容器样式 - 修复类型问题
    const wrapperStyle = computed<CSSProperties>(() => {
      return {
        position: 'relative' as const,
        width: '100%',
        minHeight: '100px'
      };
    });

    // 防篡改：监控水印DOM变化
    let observer: MutationObserver | null = null;

    onMounted(() => {
      // 防止水印被删除或修改
      observer = new MutationObserver((mutations) => {
        mutations.forEach((mutation) => {
          if (mutation.type === 'childList') {
            mutation.removedNodes.forEach((node) => {
              // 如果水印层被删除，重新添加到容器中
              if ((node as HTMLElement).classList?.contains('watermark-overlay')) {
                const wrapper = document.querySelector('.watermark-wrapper');
                if (wrapper) {
                  const newWatermark = document.createElement('div');
                  newWatermark.className = 'watermark-overlay';
                  Object.assign(newWatermark.style, watermarkStyle.value);
                  wrapper.appendChild(newWatermark);
                }
              }
            });
          }

          // 防止水印样式被修改
          if (mutation.type === 'attributes' &&
              (mutation.target as HTMLElement).classList?.contains('watermark-overlay')) {
            const watermark = mutation.target as HTMLElement;
            Object.assign(watermark.style, watermarkStyle.value);
          }
        });
      });

      // 开始观察水印元素
      const watermarkElement = document.querySelector('.watermark-overlay');
      if (watermarkElement) {
        observer.observe(watermarkElement, {
          attributes: true,
          attributeFilter: ['style', 'class']
        });

        // 同时观察父容器，防止水印被删除
        const wrapper = document.querySelector('.watermark-wrapper');
        if (wrapper) {
          observer.observe(wrapper, { childList: true });
        }
      }
    });

    onUnmounted(() => {
      if (observer) {
        observer.disconnect();
      }
    });

    return {
      watermarkStyle,
      wrapperStyle
    };
  }
});
</script>

<style scoped>
.watermark-wrapper {
  position: relative;
  width: 100%;
}

.content-slot {
  position: relative;
  z-index: 1;
}

.watermark-overlay {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  z-index: 9999;
  pointer-events: none;
}
</style>
