<template>
  <div class="wiki-shell">
    <header class="wiki-header">
      <button type="button" class="home-button" @click="goHome">
        Home
      </button>
      <div class="crumbs">
        <span class="crumb-project">
          {{ project?.title ?? projectId }}
        </span>
        <span v-if="pagePath" class="crumb-separator">/</span>
        <span v-if="pagePath" class="crumb-page">
          {{ pagePath }}
        </span>
      </div>
    </header>

    <main class="wiki-main">
      <aside class="wiki-sidebar">
        <div v-if="projectSections.length" class="sidebar-box">
          <p class="sidebar-title">Table of contents</p>
          <nav>
            <ul>
              <li class="sidebar-item level-1">
                <a href="#" @click.prevent="goToMain">
                  Main page
                </a>
              </li>
              <li
                v-for="section in projectSections"
                :key="section.pagePath"
                class="sidebar-item level-1"
              >
                <a href="#" @click.prevent="goToSection(section.pagePath)">
                  {{ section.label }}
                </a>
              </li>
            </ul>
          </nav>
        </div>

        <div v-if="headings.length" class="sidebar-box">
          <p class="sidebar-title">On this page</p>
          <nav>
            <ul>
              <li
                v-for="item in headings"
                :key="item.id"
                :class="['sidebar-item', `level-${item.level}`]"
              >
                <a :href="`#${item.id}`">
                  {{ item.title }}
                </a>
              </li>
            </ul>
          </nav>
        </div>
      </aside>

      <article class="wiki-article" v-html="rendered" @click="onClick" />
    </main>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { useRouter } from 'vue-router'
import MarkdownIt from 'markdown-it'
import markdownItAnchor from 'markdown-it-anchor'
import { projects } from '../lib/projects'

const props = defineProps<{
  projectId: string
  pagePath?: string
}>()

const router = useRouter()

function slugify(str: string): string {
  return str
    .trim()
    .toLowerCase()
    .replace(/[\s]+/g, '-')
    .replace(/[^\w-]/g, '')
}

const md = new MarkdownIt({
  html: true,
  linkify: true,
  breaks: false,
}).use(markdownItAnchor, {
  slugify,
  level: [1, 2, 3],
})

const markdownFiles = import.meta.glob(
  '../projects/**/{README.md,readme/**/*.md,readmesections/**/*.md}',
  { eager: true, import: 'default', query: '?raw' },
) as Record<string, string>

const project = computed(() =>
  projects.find(p => p.id === props.projectId),
)

const effectivePagePath = computed(() => {
  if (props.pagePath && props.pagePath.length > 0) return props.pagePath
  // default: main wiki page
  // prefer readme/README.md when it exists, otherwise root README.md
  const baseRoot = `../projects/${props.projectId}`
  if (markdownFiles[`${baseRoot}/readme/README.md`]) {
    return 'readme/README.md'
  }
  return 'README.md'
})

const markdownKey = computed(() => {
  const baseRoot = `../projects/${props.projectId}`
  return `${baseRoot}/${effectivePagePath.value}`
})

interface HeadingItem {
  id: string
  level: number
  title: string
}

const headings = computed<HeadingItem[]>(() => {
  const source = markdownFiles[markdownKey.value]
  if (!source) return []

  const tokens = md.parse(source, {})
  const items: HeadingItem[] = []

  for (let i = 0; i < tokens.length; i += 1) {
    const token = tokens[i]
    if (token.type === 'heading_open') {
      const level = Number(token.tag.slice(1))
      const inline = tokens[i + 1]
      if (!inline || inline.type !== 'inline') continue
      const title = inline.content.trim()
      if (!title) continue

      const id = slugify(title)
      items.push({ id, level, title })
    }
  }

  return items
})

interface ProjectSection {
  label: string
  pagePath: string
}

const projectSections = computed<ProjectSection[]>(() => {
  const baseRoot = `../projects/${props.projectId}/`
  const patterns: { slug: string; label: string }[] = [
    { slug: 'construction.md', label: 'Construction' },
    { slug: 'timeline.md', label: 'Timeline' },
    { slug: 'learnings.md', label: 'Learnings' },
    { slug: 'result.md', label: 'Result' },
  ]

  const keys = Object.keys(markdownFiles)
  const sections: ProjectSection[] = []

  for (const { slug, label } of patterns) {
    const candidate =
      keys.find(p => p === `${baseRoot}readme/${slug}`) ??
      keys.find(p => p === `${baseRoot}readmesections/${slug}`)
    if (!candidate) continue
    const pagePath = candidate.slice(baseRoot.length)
    sections.push({ label, pagePath })
  }

  return sections
})

const rendered = computed(() => {
  const source = markdownFiles[markdownKey.value]
  if (!source) {
    return md.render(`# Not found\n\nNo markdown page for \`${effectivePagePath.value}\`.`)
  }

  return md.render(source)
})

function goHome() {
  router.push({ name: 'dashboard' })
}

function goToMain() {
  router.push({
    name: 'wiki',
    params: {
      projectId: props.projectId,
    },
  })
}

function goToSection(pagePath: string) {
  router.push({
    name: 'wiki',
    params: {
      projectId: props.projectId,
      pagePath,
    },
  })
}

function isRelativeMarkdownLink(href: string): boolean {
  if (!href) return false
  if (href.startsWith('http://') || href.startsWith('https://')) return false
  if (href.startsWith('#')) return false
  if (href.startsWith('mailto:')) return false
  return href.endsWith('.md')
}

function resolvePagePath(href: string): string {
  // If the href is already project-root relative (e.g. "readme/..." or "readmesections/..."),
  // use it as-is instead of resolving against the current page path.
  if (href.startsWith('readme/') || href.startsWith('readmesections/')) {
    return href
  }

  // Resolve href relative to the current pagePath (effectivePagePath)
  const base = effectivePagePath.value
  const lastSlash = base.lastIndexOf('/')
  const baseDir = lastSlash >= 0 ? base.slice(0, lastSlash) : ''
  const combined = baseDir ? `${baseDir}/${href}` : href

  const segments = combined.split('/')
  const stack: string[] = []

  for (const segment of segments) {
    if (!segment || segment === '.') continue
    if (segment === '..') {
      stack.pop()
    } else {
      stack.push(segment)
    }
  }

  return stack.join('/')
}

function onClick(event: MouseEvent) {
  const target = event.target as HTMLElement | null
  if (!target) return

  if (target.tagName.toLowerCase() === 'a') {
    const el = target as HTMLAnchorElement
    const href = el.getAttribute('href') ?? ''
    if (isRelativeMarkdownLink(href)) {
      event.preventDefault()
      // Special-case links back to main README
      if (href === 'README.md' || href === '../README.md') {
        router.push({
          name: 'wiki',
          params: {
            projectId: props.projectId,
          },
        })
      } else {
        const pagePath = resolvePagePath(href)
        router.push({
          name: 'wiki',
          params: {
            projectId: props.projectId,
            pagePath,
          },
        })
      }
    }
  }
}
</script>

<style scoped>
.wiki-shell {
  min-height: 100vh;
  background: #f3e8ff;
  color: #3f3c4a;
  display: flex;
  flex-direction: column;
}

.wiki-header {
  position: sticky;
  top: 0;
  z-index: 10;
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0.85rem 2rem;
  border-bottom: 2px solid rgba(167, 139, 250, 1);
  background: #f9f5ff;
  backdrop-filter: blur(16px);
}

.home-button {
  border-radius: 0;
  border: 2px solid rgba(167, 139, 250, 1);
  padding: 0.35rem 1.1rem;
  font-size: 0.85rem;
  letter-spacing: 0.14em;
  text-transform: uppercase;
  background: white;
  color: #4c1d95;
  cursor: pointer;
}

.crumbs {
  display: flex;
  align-items: baseline;
  gap: 0.25rem;
  font-size: 0.86rem;
  color: #6b6289;
}

.crumb-project {
  text-transform: uppercase;
  letter-spacing: 0.14em;
}

.crumb-page {
  color: #4c1d95;
}

.wiki-main {
  display: flex;
  justify-content: center;
  padding: 1.75rem 2rem 3rem;
  gap: 2rem;
}

.wiki-sidebar {
  width: 220px;
  flex-shrink: 0;
  align-self: flex-start;
  position: sticky;
  top: 4.5rem;
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.sidebar-box {
  padding: 1rem 1rem 1.25rem;
  background: #fefefe;
  border: 2px solid rgba(167, 139, 250, 1);
  border-radius: 0;
}

.sidebar-title {
  margin: 0 0 0.5rem;
  font-size: 0.78rem;
  text-transform: uppercase;
  letter-spacing: 0.16em;
  color: #a855f7;
}

.wiki-sidebar ul {
  list-style: none;
  padding: 0;
  margin: 0;
}

.sidebar-item {
  margin: 0.15rem 0;
}

.sidebar-item.level-1 a {
  font-weight: 600;
}

.sidebar-item.level-2 {
  padding-left: 0.75rem;
}

.sidebar-item.level-3 {
  padding-left: 1.5rem;
}

.wiki-sidebar a {
  color: #4c1d95;
  text-decoration: none;
  font-size: 0.86rem;
}

.wiki-sidebar a:hover {
  text-decoration: underline;
}

.wiki-article {
  max-width: 780px;
  width: 100%;
  background: #fefefe;
  border-radius: 0;
  padding: 2rem 2.25rem 2.5rem;
  border: 2px solid rgba(167, 139, 250, 1);
}

.wiki-article :deep(h1),
.wiki-article :deep(h2),
.wiki-article :deep(h3),
.wiki-article :deep(h4) {
  color: #312e81;
  margin-top: 1.6rem;
  margin-bottom: 0.6rem;
  line-height: 1.25;
}

.wiki-article :deep(h1) {
  font-size: 1.9rem;
}

.wiki-article :deep(h2) {
  font-size: 1.4rem;
}

.wiki-article :deep(h3) {
  font-size: 1.1rem;
}

.wiki-article :deep(p),
.wiki-article :deep(li) {
  color: #4b5563;
  line-height: 1.7;
}

.wiki-article :deep(a) {
  color: #7c3aed;
  text-decoration: underline;
  text-decoration-thickness: 1px;
  text-underline-offset: 3px;
}

.wiki-article :deep(code) {
  background: rgba(237, 233, 254, 0.95);
  border-radius: 6px;
  padding: 0.1rem 0.4rem;
  font-size: 0.86rem;
}

.wiki-article :deep(pre code) {
  display: block;
  padding: 0.85rem 1rem;
  overflow-x: auto;
}

.wiki-article :deep(ul),
.wiki-article :deep(ol) {
  padding-left: 1.4rem;
}

.wiki-article :deep(img) {
  max-width: 100%;
  border-radius: 0;
  margin: 1rem 0;
  border: 2px solid rgba(167, 139, 250, 1);
  display: block;
}

@media (max-width: 720px) {
  .wiki-header {
    padding-inline: 1.25rem;
    flex-direction: column;
    align-items: flex-start;
    gap: 0.4rem;
  }

  .wiki-main {
    flex-direction: column;
    padding-inline: 1.25rem;
  }

  .wiki-sidebar {
    position: static;
    width: 100%;
    max-height: none;
    margin-bottom: 1.2rem;
  }

  .wiki-article {
    padding-inline: 1.4rem;
  }
}
</style>

