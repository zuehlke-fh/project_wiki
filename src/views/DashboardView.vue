<template>
  <div class="dashboard-root" ref="rootEl">
    <div v-if="years.length > 1" class="filter" ref="filterEl">
      <button
        type="button"
        class="filter-button"
        :class="{ active: selectedYear !== null }"
        aria-haspopup="true"
        :aria-expanded="filterOpen"
        @click="filterOpen = !filterOpen"
      >
        <svg viewBox="0 0 24 24" width="16" height="16" aria-hidden="true">
          <path d="M3 5h18l-7 8v6l-4 2v-8z" fill="currentColor" />
        </svg>
        {{ selectedYear ?? 'All years' }}
      </button>
      <ul v-if="filterOpen" class="filter-menu" role="menu">
        <li>
          <button
            type="button"
            role="menuitemradio"
            :aria-checked="selectedYear === null"
            :class="{ selected: selectedYear === null }"
            @click="selectYear(null)"
          >
            All years
          </button>
        </li>
        <li v-for="year in years" :key="year">
          <button
            type="button"
            role="menuitemradio"
            :aria-checked="selectedYear === year"
            :class="{ selected: selectedYear === year }"
            @click="selectYear(year)"
          >
            {{ year }}
          </button>
        </li>
      </ul>
    </div>

    <div class="tile-grid" :style="gridStyle">
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
      <div
        v-for="(name, index) in fillerNames"
        :key="`filler-${index}`"
        class="tile tile-empty"
        aria-hidden="true"
      >
        <div class="tile-image">
          <div class="tile-overlay">
            <h2 class="tile-title">{{ name }}</h2>
          </div>
        </div>
      </div>
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
import { computed, onBeforeUnmount, onMounted, ref } from 'vue'
import { useRouter } from 'vue-router'
import { projects, type Project } from '../lib/projects'
import ProjectModal from '../components/ProjectModal.vue'

const router = useRouter()

const GAP = 12 // px, matches .tile-grid gap
const TARGET_ASPECT = 1.4

const years = computed<string[]>(() =>
  Array.from(new Set(projects.map(p => p.year).filter((y): y is string => !!y))).sort().reverse(),
)

const selectedYear = ref<string | null>(null)
const filterOpen = ref(false)
const filterEl = ref<HTMLElement | null>(null)

const visibleProjects = computed<Project[]>(() =>
  selectedYear.value === null
    ? [...projects]
    : projects.filter(p => p.year === selectedYear.value),
)

function selectYear(year: string | null) {
  selectedYear.value = year
  filterOpen.value = false
}

// Choose the column count whose tiles are closest to TARGET_ASPECT so that
// every tile fits in the viewport without scrolling.
const rootEl = ref<HTMLElement | null>(null)
const viewport = ref({ width: 1200, height: 800 })

function measure() {
  const el = rootEl.value
  if (!el) return
  viewport.value = { width: el.clientWidth, height: el.clientHeight }
}

const layout = computed(() => {
  const n = Math.max(visibleProjects.value.length, 1)
  const { width, height } = viewport.value
  let best = { cols: 1, score: Number.POSITIVE_INFINITY }
  for (let cols = 1; cols <= n; cols += 1) {
    const rows = Math.ceil(n / cols)
    const w = (width - GAP * (cols - 1)) / cols
    const h = (height - GAP * (rows - 1)) / rows
    if (w <= 0 || h <= 0) continue
    const score = Math.abs(Math.log(w / h / TARGET_ASPECT))
    if (score < best.score) best = { cols, score }
  }
  const rows = Math.ceil(n / best.cols)
  return { cols: best.cols, rows, empty: best.cols * rows - visibleProjects.value.length }
})

const gridStyle = computed(() => {
  const { cols, rows } = layout.value
  return {
    gridTemplateColumns: `repeat(${cols}, minmax(0, 1fr))`,
    gridTemplateRows: `repeat(${rows}, minmax(0, 1fr))`,
    '--tile-scale': String(Math.min(1, Math.max(0.6, 5 / Math.max(cols, rows + 1)))),
  }
})

// Placeholder names for the empty grid cells, drawn from two word lists and
// re-rolled on every page load.
const NAME_PREFIXES = [
  'Silent', 'Wobbly', 'Rusty', 'Cosmic', 'Electric', 'Quantum', 'Fuzzy', 'Hidden',
  'Rapid', 'Lazy', 'Neon', 'Analog', 'Broken', 'Tiny', 'Giant', 'Magnetic',
  'Solar', 'Spare', 'Haunted', 'Turbo',
]
const NAME_SUFFIXES = [
  'Compass', 'Lantern', 'Gearbox', 'Orchestra', 'Toaster', 'Pendulum', 'Beacon',
  'Kettle', 'Resistor', 'Hamster', 'Antenna', 'Teapot', 'Capacitor', 'Windmill',
  'Spaghetti', 'Oscillator', 'Cuckoo', 'Zeppelin', 'Doorbell', 'Robot',
]

function shuffled<T>(items: T[]): T[] {
  const copy = [...items]
  for (let i = copy.length - 1; i > 0; i -= 1) {
    const j = Math.floor(Math.random() * (i + 1))
    ;[copy[i], copy[j]] = [copy[j], copy[i]]
  }
  return copy
}

// Pair the shuffled lists so no word repeats within one page load.
const fillerPool = (() => {
  const prefixes = shuffled(NAME_PREFIXES)
  const suffixes = shuffled(NAME_SUFFIXES)
  return prefixes.map((prefix, i) => `${prefix} ${suffixes[i]}`)
})()

const fillerNames = computed(() => fillerPool.slice(0, layout.value.empty))

function onDocumentClick(event: MouseEvent) {
  if (filterEl.value && !filterEl.value.contains(event.target as Node)) {
    filterOpen.value = false
  }
}

function onKeydown(event: KeyboardEvent) {
  if (event.key === 'Escape') filterOpen.value = false
}

onMounted(() => {
  measure()
  window.addEventListener('resize', measure)
  document.addEventListener('click', onDocumentClick)
  document.addEventListener('keydown', onKeydown)
})

onBeforeUnmount(() => {
  window.removeEventListener('resize', measure)
  document.removeEventListener('click', onDocumentClick)
  document.removeEventListener('keydown', onKeydown)
})

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
  position: relative;
  height: 100vh;
  height: 100dvh;
  overflow: hidden;
  background: #f9f5ff;
  padding: 0.75rem;
  box-sizing: border-box;
}

.tile-grid {
  display: grid;
  gap: 12px;
  width: 100%;
  height: 100%;
}

.tile {
  padding: 0;
  border: none;
  cursor: pointer;
  position: relative;
  overflow: hidden;
  min-width: 0;
  min-height: 0;
}

.tile-image {
  width: 100%;
  height: 100%;
  position: relative;
  transition: transform 300ms ease, filter 300ms ease;
  border: 2px solid rgba(167, 139, 250, 1);
  box-sizing: border-box;
}

.tile-empty {
  cursor: default;
  pointer-events: none;
  user-select: none;
  background: #000;
}

.tile-empty .tile-image {
  background: #000;
  border-color: #2a2a2a;
}

.tile-empty .tile-overlay {
  background: transparent;
}

.tile-empty .tile-title {
  color: #6b7280;
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
  padding: calc(1.75rem * var(--tile-scale, 1)) calc(2rem * var(--tile-scale, 1));
}

.tile-title {
  margin: 0;
  font-size: calc(1.6rem * var(--tile-scale, 1));
  letter-spacing: 0.04em;
  text-transform: uppercase;
  text-align: left;
  color: #f9fafb;
}

.filter {
  position: fixed;
  top: 1.25rem;
  right: 1.25rem;
  z-index: 20;
}

.filter-button {
  display: inline-flex;
  align-items: center;
  gap: 0.4rem;
  padding: 0.5rem 0.9rem;
  border: 2px solid rgba(167, 139, 250, 1);
  border-radius: 999px;
  background: rgba(255, 255, 255, 0.95);
  color: #4c1d95;
  font: inherit;
  font-size: 0.9rem;
  font-weight: 600;
  cursor: pointer;
  box-shadow: 0 4px 14px rgba(15, 23, 42, 0.25);
}

.filter-button:hover,
.filter-button.active {
  background: #4c1d95;
  color: #fff;
}

.filter-menu {
  list-style: none;
  margin: 0.4rem 0 0;
  padding: 0.3rem;
  min-width: 9rem;
  background: rgba(255, 255, 255, 0.98);
  border: 2px solid rgba(167, 139, 250, 1);
  border-radius: 0.75rem;
  box-shadow: 0 8px 24px rgba(15, 23, 42, 0.3);
}

.filter-menu button {
  width: 100%;
  padding: 0.45rem 0.7rem;
  border: none;
  border-radius: 0.5rem;
  background: transparent;
  color: #1f2937;
  font: inherit;
  text-align: left;
  cursor: pointer;
}

.filter-menu button:hover {
  background: #ede9fe;
}

.filter-menu button.selected {
  background: #4c1d95;
  color: #fff;
}

@media (max-width: 640px) {
  .dashboard-root {
    padding: 0.5rem;
  }

  .tile-overlay {
    padding: 0.5rem 0.6rem;
  }

  .tile-title {
    font-size: 0.8rem;
  }
}
</style>
