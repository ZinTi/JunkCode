<script setup lang="ts">
import { ref } from 'vue';
import { RouterLink, RouterView } from 'vue-router'

import AppHeader from '../components/AppHeader.vue';
import AppFooter from '../components/AppFooter.vue';
import SidebarNavigation from '../components/SidebarNavigation.vue';

const currentMenuPath = ref('');

</script>

<template>
  <div class="default-layout">
    <AppHeader />
    <div class="app-container">
      <SidebarNavigation />
      <main class="main-content">
        <RouterView />
        <!-- <p>当前选中菜单: {{ currentMenuPath }}</p> -->
      </main>
    </div>
    <AppFooter />
  </div>

</template>

<style scoped>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

.app-container {
  display: flex;
  min-height: 100vh;
}

.main-content {
  flex: 1;
  padding: 0px;
  background-color: #f5f7fa;
  overflow-y: auto;
}

</style>
