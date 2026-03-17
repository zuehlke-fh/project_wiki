<template>
  <div class="backdrop" @click.self="$emit('close')">
    <div class="panel">
      <header class="panel-header">
        <div>
          <p class="label">Project</p>
          <h1>{{ project.title }}</h1>
        </div>
        <button type="button" class="icon-button" @click="$emit('close')">
          ✕
        </button>
      </header>

      <section class="content">
        <p class="description">
          {{ project.about?.description?.short ?? 'No description provided yet.' }}
        </p>

        <div v-if="project.about?.stats" class="stats-grid">
          <div v-for="(value, key) in flatStats" :key="key" class="stat">
            <p class="stat-label">
              {{ key }}
            </p>
            <p class="stat-value">
              {{ value }}
            </p>
          </div>
        </div>

        <div v-if="keywords.length" class="keywords">
          <span
            v-for="word in keywords"
            :key="word"
            class="chip"
          >
            {{ word }}
          </span>
        </div>
      </section>

      <footer class="panel-footer">
        <button type="button" class="secondary" @click="$emit('close')">
          Close
        </button>
        <button type="button" class="primary" @click="$emit('open-wiki', project)">
          Open wiki
        </button>
      </footer>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import type { Project } from '../lib/projects'

const props = defineProps<{
  project: Project
}>()

defineEmits<{
  close: []
  'open-wiki': [project: Project]
}>()

const flatStats = computed<Record<string, string>>(() => {
  const src = props.project.about?.stats ?? {}
  const result: Record<string, string> = {}
  for (const [key, value] of Object.entries(src)) {
    if (Array.isArray(value)) continue
    if (value == null) continue
    result[key.replace(/-/g, ' ')] = String(value)
  }
  return result
})

const keywords = computed<string[]>(() => {
  const raw = props.project.about?.stats?.keywords
  return Array.isArray(raw) ? raw : []
})
</script>

<style scoped>
.backdrop {
  position: fixed;
  inset: 0;
  background: rgba(15, 23, 42, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 2rem;
  z-index: 40;
}

.panel {
  width: min(720px, 100%);
  background: #fefefe;
  border-radius: 0;
  border: 2px solid rgba(167, 139, 250, 1);
  padding: 1.75rem 2rem 1.5rem;
  color: #e5e7eb;
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.panel-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 1rem;
}

.label {
  text-transform: uppercase;
  font-size: 0.75rem;
  letter-spacing: 0.18em;
  color: #6b6289;
  margin: 0 0 0.2rem;
}

h1 {
  margin: 0;
  font-size: 1.75rem;
  letter-spacing: 0.04em;
  color: #312e81;
}

.icon-button {
  border-radius: 0;
  border: 2px solid rgba(167, 139, 250, 1);
  width: 36px;
  height: 36px;
  background: #f9f5ff;
  color: #4c1d95;
  display: grid;
  place-items: center;
  cursor: pointer;
}

.icon-button:hover {
  background: #ede9fe;
}

.content {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.description {
  margin: 0;
  line-height: 1.6;
  color: #4b5563;
}

.stats-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(140px, 1fr));
  gap: 0.75rem 1.25rem;
}

.stat-label {
  margin: 0;
  font-size: 0.75rem;
  text-transform: uppercase;
  letter-spacing: 0.12em;
  color: #6b7280;
}

.stat-value {
  margin: 0.15rem 0 0;
  color: #111827;
}

.keywords {
  display: flex;
  flex-wrap: wrap;
  gap: 0.5rem;
}

.chip {
  border-radius: 0;
  padding: 0.2rem 0.75rem;
  font-size: 0.8rem;
  letter-spacing: 0.08em;
  text-transform: uppercase;
  background: #111827;
  border: 2px solid #111827;
  color: #f9fafb;
}

.panel-footer {
  display: flex;
  justify-content: flex-end;
  gap: 0.75rem;
}

.primary,
.secondary {
  border-radius: 0;
  padding: 0.5rem 1.4rem;
  font-size: 0.9rem;
  letter-spacing: 0.12em;
  text-transform: uppercase;
  border: 1px solid transparent;
  cursor: pointer;
}

.secondary {
  background: transparent;
  border-color: rgba(167, 139, 250, 1);
  color: #4c1d95;
}

.primary {
  background: #4c1d95;
  color: #f9fafb;
}

.primary:hover {
  filter: brightness(1.05);
}

@media (max-width: 640px) {
  .backdrop {
    padding: 1rem;
  }

  .panel {
    padding-inline: 1.25rem;
  }

  h1 {
    font-size: 1.4rem;
  }

  .panel-footer {
    flex-direction: column-reverse;
    align-items: stretch;
  }

  .primary,
  .secondary {
    width: 100%;
    justify-content: center;
  }
}
</style>

