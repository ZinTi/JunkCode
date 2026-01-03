<!-- components/SidebarNavigation.vue -->
<template>
  <nav class="sidebar" :class="{ 'sidebar-collapsed': isCollapsed }">
    <!-- 折叠按钮 -->
    <div class="sidebar-header">
      <h2 v-show="!isCollapsed">功能导航</h2>
      <button class="collapse-btn" @click="toggleSidebar">
        {{ isCollapsed ? '→' : '←' }}
      </button>
    </div>

    <!-- 菜单列表 -->
    <ul class="menu-list">
      <li
        v-for="menu in menus"
        :key="menu.id"
        class="menu-item"
        :class="{ active: activeMenuId === menu.id }"
      >
        <!-- 一级菜单 -->
        <div
          class="menu-title"
          @click="toggleSubMenu(menu.id)"
        >
          <span class="menu-icon">{{ menu.icon }}</span>
          <span v-show="!isCollapsed" class="menu-text">{{ menu.title }}</span>
          <span
            v-if="menu.children && menu.children.length > 0 && !isCollapsed"
            class="arrow"
            :class="{ 'arrow-expanded': menu.isExpanded }"
          >
            ▼
          </span>
        </div>

        <!-- 二级菜单 -->
        <transition name="submenu">
          <ul
            v-if="menu.children && menu.children.length > 0 && menu.isExpanded && !isCollapsed"
            class="submenu-list"
          >
            <li
              v-for="subMenu in menu.children"
              :key="subMenu.id"
              class="submenu-item"
              :class="{ active: activeSubMenuId === subMenu.id }"
              @click="selectSubMenu(menu.id, subMenu.id, subMenu.path)"
            >
              <span class="submenu-icon"></span>
              <span class="submenu-text">{{ subMenu.title }}</span>
            </li>
          </ul>
        </transition>
      </li>
    </ul>
  </nav>
</template>

<script setup lang="ts">
import { ref, reactive, onMounted } from 'vue';

interface MenuItem {
  id: number;
  title: string;
  icon: string;
  isExpanded?: boolean;
  children?: SubMenuItem[];
}

interface SubMenuItem {
  id: number;
  parentId: number;
  title: string;
  path: string;
}

// 响应式状态
const isCollapsed = ref(false);
const activeMenuId = ref<number | null>(null);
const activeSubMenuId = ref<number | null>(null);

// 菜单数据
const menus = reactive<MenuItem[]>([
  {
    id: 1,
    title: '个人信息',
    icon: '👤',
    isExpanded: false,
    children: [
      { id: 101, parentId: 1, title: '个人信息管理', path: '/profile/manage' },
      { id: 102, parentId: 1, title: '注销账号', path: '/profile/deactivate' }
    ]
  },
  {
    id: 2,
    title: '业务管理1',
    icon: '📁',
    isExpanded: false,
    children: [
      { id: 201, parentId: 2, title: '业务信息1-1', path: '/business1/info1' },
      { id: 202, parentId: 2, title: '业务信息1-2', path: '/business1/info2' },
      { id: 203, parentId: 2, title: '业务信息1-3', path: '/business1/info3' }
    ]
  },
  {
    id: 3,
    title: '业务管理2',
    icon: '📂',
    isExpanded: false,
    children: [
      { id: 301, parentId: 3, title: '业务信息2-1', path: '/business2/info1' },
      { id: 302, parentId: 3, title: '业务信息2-2', path: '/business2/info2' },
      { id: 303, parentId: 3, title: '业务信息2-3', path: '/business2/info3' }
    ]
  }
]);

// 切换侧边栏折叠状态
const toggleSidebar = () => {
  isCollapsed.value = !isCollapsed.value;
  // 折叠时收起所有子菜单
  if (isCollapsed.value) {
    menus.forEach(menu => {
      menu.isExpanded = false;
    });
  }
};

// 切换子菜单展开状态
const toggleSubMenu = (menuId: number) => {
  if (isCollapsed.value) {
    isCollapsed.value = false;
    setTimeout(() => {
      const menu = menus.find(m => m.id === menuId);
      if (menu && menu.children && menu.children.length > 0) {
        const shouldExpand = !menu.isExpanded;
        menus.forEach(m => {
          m.isExpanded = false;
        });
        menu.isExpanded = shouldExpand;

        // 添加类型检查
        if (shouldExpand) {
          const firstChild = menu.children[0];
          if (firstChild) {
            selectSubMenu(menuId, firstChild.id, firstChild.path);
          }
        }
      }
    }, 50);
    return;
  }

  const menu = menus.find(m => m.id === menuId);
  if (menu) {
    const shouldExpand = !menu.isExpanded;
    menus.forEach(m => {
      m.isExpanded = false;
    });
    menu.isExpanded = shouldExpand;

    // 添加类型检查
    if (menu.children && menu.children.length > 0 && shouldExpand) {
      const firstChild = menu.children[0];
      if (firstChild) {
        selectSubMenu(menuId, firstChild.id, firstChild.path);
      }
    }
  }
};

// 选择子菜单
const selectSubMenu = (menuId: number, subMenuId: number, path: string) => {
  activeMenuId.value = menuId;
  activeSubMenuId.value = subMenuId;
  console.log(`导航到: ${path}`);
  // 这里可以触发路由跳转或emit事件
  // emit('menu-change', { menuId, subMenuId, path });
};

// 初始化选中第一个菜单的第一个子项
onMounted(() => {
  if (menus[0] && menus[0].children && menus[0].children.length > 0) {
    const firstChild = menus[0].children[0];
    if (firstChild) {
      selectSubMenu(menus[0].id, firstChild.id, firstChild.path);
    }
  }
});
</script>

<style scoped>
.sidebar {
  width: 250px;
  background: linear-gradient(180deg, #1e3c72 0%, #2a5298 100%);
  color: white;
  transition: all 0.3s ease;
  overflow-y: auto;
  overflow-x: hidden;
  display: flex;
  flex-direction: column;
  box-shadow: 2px 0 10px rgba(0, 0, 0, 0.1);
}

.sidebar-collapsed {
  width: 60px;
}

.sidebar-header {
  padding: 20px 15px;
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
  display: flex;
  justify-content: space-between;
  align-items: center;
  min-height: 80px;
}

.sidebar-header h2 {
  font-size: 1.2rem;
  font-weight: 600;
  white-space: nowrap;
  margin: 0;
}

.collapse-btn {
  background: rgba(255, 255, 255, 0.1);
  border: none;
  color: white;
  width: 30px;
  height: 30px;
  border-radius: 4px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.3s ease;
  font-size: 12px;
}

.collapse-btn:hover {
  background: rgba(255, 255, 255, 0.2);
  transform: scale(1.1);
}

.menu-list {
  list-style: none;
  padding: 20px 0;
  flex: 1;
}

.menu-item {
  margin: 5px 10px;
  border-radius: 8px;
  overflow: hidden;
  transition: all 0.3s ease;
}

.menu-item.active {
  background: rgba(255, 255, 255, 0.15);
}

.menu-item:hover:not(.active) {
  background: rgba(255, 255, 255, 0.05);
}

.menu-title {
  padding: 12px 15px;
  display: flex;
  align-items: center;
  cursor: pointer;
  user-select: none;
  transition: all 0.3s ease;
}

.menu-title:hover {
  background: rgba(255, 255, 255, 0.05);
}

.menu-icon {
  font-size: 18px;
  margin-right: 12px;
  min-width: 20px;
  text-align: center;
}

.menu-text {
  flex: 1;
  font-size: 14px;
  font-weight: 500;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.arrow {
  font-size: 10px;
  transition: transform 0.3s ease;
  transform: rotate(-90deg);
  opacity: 0.7;
}

.arrow-expanded {
  transform: rotate(0deg);
}

/* 二级菜单样式 */
.submenu-list {
  list-style: none;
  padding: 5px 0 5px 40px;
  background: rgba(0, 0, 0, 0.1);
  border-radius: 0 0 8px 8px;
}

.submenu-item {
  padding: 10px 15px;
  cursor: pointer;
  transition: all 0.2s ease;
  border-radius: 6px;
  margin: 2px 0;
  display: flex;
  align-items: center;
  font-size: 13px;
  opacity: 0.9;
}

.submenu-item:hover {
  background: rgba(255, 255, 255, 0.1);
  opacity: 1;
}

.submenu-item.active {
  background: rgba(255, 255, 255, 0.2);
  color: #fff;
  font-weight: 500;
  position: relative;
}

.submenu-item.active::before {
  content: '';
  position: absolute;
  left: 0;
  top: 50%;
  transform: translateY(-50%);
  width: 3px;
  height: 60%;
  background: #4dabf7;
  border-radius: 0 2px 2px 0;
}

.submenu-icon {
  display: inline-block;
  width: 4px;
  height: 4px;
  background: rgba(255, 255, 255, 0.6);
  border-radius: 50%;
  margin-right: 10px;
}

.submenu-item.active .submenu-icon {
  background: #4dabf7;
}

.submenu-text {
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

/* 折叠状态下的样式 */
.sidebar-collapsed .menu-title {
  justify-content: center;
  padding: 15px 0;
}

.sidebar-collapsed .menu-icon {
  margin-right: 0;
  font-size: 20px;
}

.sidebar-collapsed .menu-text,
.sidebar-collapsed .arrow {
  display: none;
}

/* 动画效果 */
.submenu-enter-active,
.submenu-leave-active {
  transition: all 0.3s ease;
  max-height: 200px;
  overflow: hidden;
}

.submenu-enter-from,
.submenu-leave-to {
  max-height: 0;
  opacity: 0;
  transform: translateY(-10px);
}
</style>
