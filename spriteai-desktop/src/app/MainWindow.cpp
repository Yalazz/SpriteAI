#include "app/MainWindow.h"
#include "app/CanvasWidget.h"

#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(1280, 720);

    m_canvas = new CanvasWidget(this);
    setCentralWidget(m_canvas);

    // İlk başlık (çeviri gelmeden önce)
    setWindowTitle(tr("Sprite AI Desktop"));
}

MainWindow::~MainWindow() = default;

void MainWindow::setLanguage(const QString& lang)
{
    qApp->removeTranslator(&translator);

    if (translator.load(":/i18n/spriteai_" + lang))
    {
        qApp->installTranslator(&translator);
    }

    // ❗ UI METİNLERİNİ YENİDEN SET ET
    setWindowTitle(tr("Sprite AI Desktop"));
}
