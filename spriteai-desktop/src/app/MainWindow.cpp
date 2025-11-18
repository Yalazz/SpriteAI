#include "MainWindow.h"
#include "CanvasWidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("SpriteAI Desktop");
    resize(1280, 720);

    m_canvas = new CanvasWidget(this);
    setCentralWidget(m_canvas);
}

MainWindow::~MainWindow() = default;
