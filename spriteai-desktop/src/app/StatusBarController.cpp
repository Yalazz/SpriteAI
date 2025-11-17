#include "StatusBarController.h"
#include <QStatusBar>

StatusBarController::StatusBarController(QObject* parent)
    : QObject(parent)
{
}

void StatusBarController::attach(QStatusBar* statusBar)
{
    m_statusBar = statusBar;
}

void StatusBarController::showStartupMessage()
{
    if (!m_statusBar) return;
    m_statusBar->showMessage("Sprite AI başlatılıyor...");
}

void StatusBarController::showInfo(const QString& message)
{
    if (!m_statusBar) return;
    m_statusBar->showMessage(message, 5000);
}

void StatusBarController::showError(const QString& message)
{
    if (!m_statusBar) return;
    m_statusBar->showMessage(message);
}
