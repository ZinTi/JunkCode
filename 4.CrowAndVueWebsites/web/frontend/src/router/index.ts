import { createRouter, createWebHistory } from 'vue-router'
// import HomeView from '../views/HomeView.vue'
import DefaultLayout from '../layouts/DefaultLayout.vue'

// 扩展路由元信息类型
declare module 'vue-router' {
  interface RouteMeta {
    title?: string
    watermark_text?: string
    // requiresAuth?: boolean
    // 其他元信息字段
  }
}

const router = createRouter({
  // history: createWebHistory(import.meta.env.BASE_URL),
  history: createWebHistory('/app'), // 前端路由 base 路径，不要设置 "./app"，不然每次刷新都会一直在尾部追加 /app/
  routes: [
    {
      path: '/signup',
      name: 'signup',
      component: () => import('../views/SignUpView.vue'),
      meta: {
        title: '注册页 - Websites(1/10)',
        watermark_text: '1/10 注册页面（张宇川-20221054）'
      }
    },
    {
      path: '/login',
      name: 'login',
      component: () => import('../views/LogInView.vue'),
      meta: {
        title: '登录页 - Websites(2/10)',
        watermark_text: "2/10 登录页面（张宇川-20221054）"
      }
    },
    {
      path: '/',
      name: 'home',
      component: DefaultLayout,
      children: [
        {
          path: '',
          component: () => import('../views/HomeView.vue')
        }
      ],
      meta: {
        title: '首页 - Websites(3/10)'
      }
    },
    {
      path: '/about',
      name: 'about',
      component: DefaultLayout,
      children: [
        {
          path: '',
          component: () => import('../views/AboutView.vue')
        }
      ],
      meta: {
        title: '关于 - Websites(4/10)',
        watermark_text: "4/10 关于页面（张宇川-20221054）"
      }
    },
    {
      path: '/profile',
      name: 'profile',
      component: DefaultLayout,
      children: [
        {
          path: '/manage',
          component: () => import('../views/MyInfo.vue'),
          meta: {
            title: '个人信息 - Websites(5/10)',
            watermark_text: "5/10 个人信息页面（张宇川-20221054）"
          }
        },
        {
          path: '/deactivate',
          component: () => import('../views/DeleteAccount.vue'),
          meta: {
            title: '注销账号 - Websites(6/10)',
            watermark_text: "6/10 注销账号页面（张宇川-20221054）"
          }
        }
      ]
    },
    {
      path: '/contact',
      name: 'contact',
      component: DefaultLayout,
      children: [
        {
          path: '',
          component: () => import('../views/ContactView.vue')
        }
      ],
      meta: {
        title: '反馈 - Websites(7/10)',
        watermark_text: "7/10 联系我们（张宇川-20221054）"
      }
    },
  ],
})

// 设置标题的全局守卫
router.beforeEach((to, from, next) => {
  // 使用类型断言或条件判断
  if (to.meta.title && typeof to.meta.title === 'string') {
    document.title = to.meta.title
  } else {
    document.title = 'Websites 默认标题'  // 提供默认值
  }
  next()
})


export default router
