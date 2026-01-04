#pragma once

#include <QObject>
#include <QDockWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class ToolsDock; }
QT_END_NAMESPACE

namespace spriteai::editor::app {

class ToolsDockWidget final : public QDockWidget {
    Q_OBJECT

public:
    explicit ToolsDockWidget(QWidget* parent = nullptr);
    ~ToolsDockWidget() override;

    void setCurrentTool(const QString& toolId);

signals:
    void toolSelected(const QString& toolId);

private slots:
    void onBrushClicked();
    void onPencilClicked();
    void onEraserClicked();
    void onFillClicked();
    void onEyedropperClicked();
    void onSelectClicked();
    void onMoveClicked();
    void onAIClicked();

private:
    void setupConnections();

    std::unique_ptr<Ui::ToolsDock> ui;
};

} // namespace spriteai::editor::app
