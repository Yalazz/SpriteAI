#pragma once

#include <QMainWindow>

class StatusBarController;
class VulkanEngine;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    StatusBarController* m_statusBarController = nullptr;
    VulkanEngine*        m_vulkanEngine        = nullptr;

    void initializeUi();
    void initializeVulkan();
};
