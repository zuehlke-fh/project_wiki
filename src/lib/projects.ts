export interface ProjectAboutStats {
  weight?: string
  cost?: string
  'found-component'?: string
  microcontroller?: string
  keywords?: string[]
  [key: string]: unknown
}

export interface ProjectAbout {
  description?: {
    short?: string
  }
  stats?: ProjectAboutStats
  image?: string
  title?: string
  [key: string]: unknown
}

export interface Project {
  id: string
  title: string
  rootPath: string
  about?: ProjectAbout
  heroImage?: string
}

// Load all about.json files eagerly so we can build a project list at startup
const aboutFiles = import.meta.glob('../projects/**/about.json', {
  eager: true,
  import: 'default',
}) as Record<string, ProjectAbout>

// Discover all project root folders (by any file inside them)
const projectRootFiles = import.meta.glob('../projects/project-documentation-*/*', {
  eager: false,
})

function prettifyId(id: string): string {
  return id
    .replace(/^project-documentation-/, '')
    .split(/[-_]/)
    .map(part => part.charAt(0).toUpperCase() + part.slice(1))
    .join(' ')
}

const imageFiles = import.meta.glob('../projects/**/{images,Images}/*.{png,jpg,jpeg,gif,webp,svg}', {
  eager: true,
  import: 'default',
}) as Record<string, string>

const projectIds = Array.from(
  new Set(
    Object.keys(projectRootFiles)
      .map(path => path.split('/').find(part => part.startsWith('project-documentation-')))
      .filter((id): id is string => !!id),
  ),
)

export const projects: Project[] = projectIds.map(id => {
  const rootPath = id
  const aboutEntry =
    Object.entries(aboutFiles).find(([path]) => path.includes(`/${id}/about.json`))?.[1] ?? undefined

  const title = aboutEntry?.title ?? prettifyId(id)

  const basePrefix = `../projects/${id}/`
  const candidateImages = Object.entries(imageFiles)
    .filter(([imgPath]) => imgPath.startsWith(basePrefix))
    .map(([, url]) => url)

  const heroImage = candidateImages.length > 0 ? candidateImages[0] : undefined

  return {
    id,
    title,
    rootPath,
    about: aboutEntry,
    heroImage,
  }
})

