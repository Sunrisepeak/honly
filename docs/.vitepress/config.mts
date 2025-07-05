import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "HOnly",
  base: "/honly/",
  description: "A HOnly Website",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: '主页', link: '/' },
      { text: '文档', link: '/libs/demo/intro.md' },
      {
        text: 'd2learn社区',
        items: [
          { text: 'd2learn论坛', link: 'https://forum.d2learn.org' },
          { text: 'd2learn开源', link: 'https://github.com/d2learn' },
          { text: 'd2learn主页', link: 'https://d2learn.org' }
        ]
      }
    ],

    sidebar: [
      {
        text: '快速开始',
        collapsed: false,
        items: [
          { text: '介绍', link: '/libs/demo/intro.md' },
          { text: '基本用法', link: '/libs/demo/example.md' }
        ]
      },
      {
        text: 'JiaMi',
        collapsed: true,
        items: [
          { text: '介绍', link: '/libs/jiami/intro.md' },
          { text: '基本用法', link: '/libs/jiami/example.md' }
        ]
      },
      {
        text: 'Log Macro ',
        collapsed: true,
        items: [
          { text: '介绍', link: '/libs/logmacro/intro.md' },
          { text: '基本用法', link: '/libs/logmacro/example.md' }
        ]
      },
      {
        text: 'OpenGL Recorder',
        collapsed: true,
        items: [
          { text: '介绍', link: '/libs/opengl-recorder/intro.md' },
          { text: '基本用法', link: '/libs/opengl-recorder/example.md' }
        ]
      },
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/Sunrisepeak/honly/docs' }
    ]
  }
})
