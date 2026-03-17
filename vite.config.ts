import { defineConfig } from 'vite'
import path from 'node:path'
import vue from '@vitejs/plugin-vue'

// https://vite.dev/config/
export default defineConfig({
  plugins: [vue()],
  base: '/project_wiki/',
  resolve: {
    alias: {
      '@docs': path.resolve(__dirname, '../project-documentation-submissions'),
    },
  },
})
