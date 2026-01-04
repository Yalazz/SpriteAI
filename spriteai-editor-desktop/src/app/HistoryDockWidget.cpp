#include "spriteai/editor/app/HistoryDockWidget.h"
#include "ui_HistoryDock.h"

namespace spriteai::editor::app {

HistoryDockWidget::HistoryDockWidget(QWidget* parent)
    : QDockWidget(parent)
    , ui(std::make_unique<Ui::HistoryDock>())
{
    auto* widget = new QWidget(this);
    ui->setupUi(widget);
    setWidget(widget);

    setupConnections();
}

HistoryDockWidget::~HistoryDockWidget() = default;

void HistoryDockWidget::setupConnections()
{
    connect(ui->btnUndo, &QToolButton::clicked, this, &HistoryDockWidget::onUndo);
    connect(ui->btnRedo, &QToolButton::clicked, this, &HistoryDockWidget::onRedo);
    connect(ui->btnClearHistory, &QToolButton::clicked, this, &HistoryDockWidget::onClearHistory);

    connect(ui->listHistory, &QListWidget::currentRowChanged, this, &HistoryDockWidget::onHistoryItemClicked);
}

void HistoryDockWidget::addHistoryEntry(const QString& action, const QString& iconPath)
{
    QIcon icon;
    if (!iconPath.isEmpty()) {
        icon = QIcon(iconPath);
    } else {
        icon = QIcon(":/icons/icons/history.svg");
    }

    auto* item = new QListWidgetItem(icon, action);
    ui->listHistory->addItem(item);
    ui->listHistory->setCurrentRow(ui->listHistory->count() - 1);
}

void HistoryDockWidget::setCurrentIndex(int index)
{
    if (index >= 0 && index < ui->listHistory->count()) {
        ui->listHistory->setCurrentRow(index);
    }
}

void HistoryDockWidget::clearHistory()
{
    ui->listHistory->clear();
    auto* item = new QListWidgetItem(
        QIcon(":/icons/icons/new.svg"),
        tr("New Document")
    );
    ui->listHistory->addItem(item);
    ui->listHistory->setCurrentRow(0);
}

void HistoryDockWidget::onUndo()
{
    emit undoRequested();
}

void HistoryDockWidget::onRedo()
{
    emit redoRequested();
}

void HistoryDockWidget::onClearHistory()
{
    clearHistory();
    emit historyCleared();
}

void HistoryDockWidget::onHistoryItemClicked(int row)
{
    emit historyIndexChanged(row);
}

} // namespace spriteai::editor::app
