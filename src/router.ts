import { createRouter, createWebHistory } from 'vue-router'
import type { RouteRecordRaw } from 'vue-router'
import DashboardView from './views/DashboardView.vue'
import WikiView from './views/WikiView.vue'

const routes: RouteRecordRaw[] = [
  {
    path: '/',
    name: 'dashboard',
    component: DashboardView,
  },
  {
    path: '/wiki/:projectId/:pagePath(.*)?',
    name: 'wiki',
    component: WikiView,
    props: route => ({
      projectId: route.params.projectId,
      pagePath: Array.isArray(route.params.pagePath)
        ? route.params.pagePath.join('/')
        : route.params.pagePath,
    }),
  },
]

export const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes,
})

