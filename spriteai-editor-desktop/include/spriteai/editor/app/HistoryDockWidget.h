#pragma once

#include <QObject>
#include <QDockWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class HistoryDock; }
QT_END_NAMESPACE

namespace spriteai::editor::app {

class HistoryDockWidget final : public QDockWidget {
    Q_OBJECT

public:
    explicit HistoryDockWidget(QWidget* parent = nullptr);
    ~HistoryDockWidget() override;

    void addHistoryEntry(const QString& action, const QString& iconPath = QString());
    void setCurrentIndex(int index);
    void clearHistory();

signals:
    void undoRequested();
    void redoRequested();
    void historyCleared();
    void historyIndexChanged(int index);

private slots:
    void onUndo();
    void onRedo();
    void onClearHistory();
    void onHistoryItemClicked(int row);

private:
    void setupConnections();

    std::unique_ptr<Ui::HistoryDock> ui;
};

} // namespace spriteai::editor::app
