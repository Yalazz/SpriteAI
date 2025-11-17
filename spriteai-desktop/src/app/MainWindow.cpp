#include "MainWindow.h"
#include "StatusBarController.h"
#include "vulkan/VulkanEngine.h"

#include <QVBoxLayout>
#include <QLabel>
#include <iostream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    initializeUi();
    initializeVulkan();
}

MainWindow::~MainWindow()
{
    if (m_vulkanEngine) {
        m_vulkanEngine->shutdown();
        delete m_vulkanEngine;
    }
    delete m_statusBarController;
}

void MainWindow::initializeUi()
{
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    auto* label = new QLabel("Sprite AI Desktop — Vulkan Test", this);
    layout->addWidget(label);

    setCentralWidget(central);

    // 🔥 StatusBarController doğru şekilde bağlanıyor
    m_statusBarController = new StatusBarController(this);
    m_statusBarController->attach(statusBar());
    m_statusBarController->showStartupMessage();
}

void MainWindow::initializeVulkan()
{
    try
    {
        m_vulkanEngine = new VulkanEngine();
        m_vulkanEngine->initialize();

        m_statusBarController->showInfo("Vulkan başarıyla başlatıldı.");
    }
    catch (const std::exception& e)
    {
        QString err = QString("Vulkan hata: %1").arg(e.what());
        m_statusBarController->showError(err);
        std::cerr << err.toStdString() << std::endl;
    }
}
