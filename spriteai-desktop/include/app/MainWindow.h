#pragma once

#include <QMainWindow>
#include <QTranslator>

class CanvasWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:
    void setLanguage(const QString& lang);   // "tr" / "en"

private:
    CanvasWidget* m_canvas = nullptr;
    QTranslator   translator;
};
