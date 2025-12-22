#pragma once

#include <QMainWindow>

class CanvasWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    CanvasWidget* m_canvas = nullptr;
};
