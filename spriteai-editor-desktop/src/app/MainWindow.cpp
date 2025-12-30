#include "spriteai/editor/app/MainWindow.h"
#include "spriteai/editor/app/CanvasWidget.h"

namespace spriteai::editor::app {

    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        setWindowTitle("Sprite AI");
        setCentralWidget(new CanvasWidget(this));
        resize(900, 600);
    }

    MainWindow::~MainWindow() = default;

} // namespace spriteai::editor::app
