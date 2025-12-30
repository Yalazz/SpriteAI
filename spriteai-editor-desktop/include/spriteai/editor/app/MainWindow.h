#pragma once

#include <QObject>        // 🔥 ZORUNLU
#include <QMainWindow>

namespace spriteai::editor::app {

    class MainWindow final : public QMainWindow {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow() override;
    };

}
