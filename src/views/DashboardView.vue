<template>
  <div class="dashboard-root">
    <div class="tile-grid">
      <button
        v-for="project in visibleProjects"
        :key="project.id"
        class="tile"
        type="button"
        @click="openProject(project)"
      >
        <div class="tile-image" :style="backgroundStyle(project)">
          <div class="tile-overlay">
            <h2 class="tile-title">
              {{ project.title }}
            </h2>
          </div>
        </div>
      </button>
    </div>

    <ProjectModal
      v-if="selected"
      :project="selected"
      @close="selected = null"
      @open-wiki="goToWiki"
    />
  </div>
</template>

<script setup lang="ts">
import { computed, ref } from 'vue'
import { useRouter } from 'vue-router'
import { projects, type Project } from '../lib/projects'
import ProjectModal from '../components/ProjectModal.vue'

const router = useRouter()

const visibleProjects = computed<Project[]>(() => [...projects])

const selected = ref<Project | null>(null)

function backgroundStyle(project: Project) {
  if (!project.heroImage) {
    return {
      background:
        'radial-gradient(circle at top left, rgba(255,255,255,0.7), rgba(148,163,184,0.4))',
    }
  }

  return {
    backgroundImage: `url(${project.heroImage})`,
    backgroundSize: 'cover',
    backgroundPosition: 'center',
  }
}

function openProject(project: Project) {
  selected.value = project
}

function goToWiki(project: Project) {
  router.push({
    name: 'wiki',
    params: {
      projectId: project.id,
    },
  })
}
</script>

<style scoped>
.dashboard-root {
  min-height: 100vh;
  background: #f9f5ff;
  display: flex;
  align-items: stretch;
  justify-content: center;
  padding: 0.75rem;
  box-sizing: border-box;
}

.tile-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  grid-auto-rows: calc(50vh - 0.75rem - 0.375rem);
  gap: 0.75rem;
  width: 100%;
}

.tile {
  padding: 0;
  border: none;
  cursor: pointer;
  position: relative;
  overflow: hidden;
}

.tile-image {
  width: 100%;
  height: 100%;
  position: relative;
  transition: transform 300ms ease, filter 300ms ease;
  border: 2px solid rgba(167, 139, 250, 1);
}

.tile::before {
  content: '';
  position: absolute;
  inset: 0;
  border: 1px solid rgba(15, 23, 42, 0.6);
  mix-blend-mode: soft-light;
  pointer-events: none;
}

.tile:hover .tile-image {
  transform: scale(1.03);
  filter: brightness(1.05);
}

.tile-overlay {
  position: absolute;
  inset: 0;
  background: rgba(15, 23, 42, 0.55);
  display: flex;
  align-items: flex-end;
  padding: 1.75rem 2rem;
}

.tile-title {
  margin: 0;
  font-size: 1.6rem;
  letter-spacing: 0.04em;
  text-transform: uppercase;
  color: #f9fafb;
}

@media (max-width: 960px) {
  .tile-grid {
    grid-template-columns: repeat(2, minmax(0, 1fr));
    grid-auto-rows: 160px;
    aspect-ratio: auto;
  }

  .tile-overlay {
    padding: 1rem 1.25rem;
  }

  .tile-title {
    font-size: 1.2rem;
  }
}

@media (max-width: 640px) {
  .tile-grid {
    grid-template-columns: repeat(1, minmax(0, 1fr));
    grid-auto-rows: 140px;
  }
}
</style>

