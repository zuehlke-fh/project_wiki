# Project Wiki

Wiki of previous student projects, built with Vue 3 + TypeScript + Vite and deployed to GitHub Pages on every push to `main`.

## Develop

```bash
npm ci
npm run dev      # local dev server
npm run build    # type-check + production build into dist/
```

## Adding a project

Create `src/projects/project-documentation-<name>/` containing:

```
about.json        # description.short and stats (weight, cost, microcontroller, keywords)
images/           # all images; reference them from markdown as ../images/<file>
readme/           # README.md, construction.md, learnings.md, result.md, timeline.md
```

Projects are discovered automatically. `public/projects/<name>/images` is a symlink to
`src/projects/<name>/images` so the images exist in one place only; create the same
symlink for a new project:

```bash
ln -s ../../../src/projects/<name>/images public/projects/<name>/images
```
