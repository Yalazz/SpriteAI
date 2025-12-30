# spriteai-editor-desktop

Qt6 editor. `spriteai-engine` repoyu workspace içinde sibling olarak bekler.

- i18n: Qt Linguist (.ts/.qm) + QTranslator
- Input: Qt events -> engine ToolInput (screen->world)
- Render: CPU renderer buffer -> QImage -> QPainter
