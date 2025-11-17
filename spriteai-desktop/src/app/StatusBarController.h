#pragma once

#include <QObject>
#include <QString>

class QStatusBar;

class StatusBarController : public QObject
{
    Q_OBJECT

public:
    explicit StatusBarController(QObject* parent = nullptr);

    void attach(QStatusBar* statusBar);

    void showStartupMessage();
    void showInfo(const QString& message);
    void showError(const QString& message);

private:
    QStatusBar* m_statusBar = nullptr;
};
