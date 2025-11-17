#include "MainWindow.h"
#include "StatusBarController.h"
#include "../vulkan/VulkanEngine.h"

#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_statusBarController(new StatusBarController(this))
    , m_vulkanEngine(new VulkanEngine())
{
    initializeUi();
    initializeVulkan();
}

MainWindow::~MainWindow()
{
    delete m_vulkanEngine;
    m_vulkanEngine = nullptr;
}

void MainWindow::initializeUi()
{
    auto* centralWidget = new QWidget(this);
    auto* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(16, 16, 16, 16);

    auto* infoLabel = new QLabel(
        "Sprite AI - Desktop Editor Skeleton\n\n"
        "Bu iskelet, Sprite AI masaüstü editörünün başlangıç noktasıdır.\n"
        "VulkanEngine şu anda yalnızca Vulkan instance ve device oluşturur.\n"
        "Bir sonraki adımda swapchain, render pass ve gerçek canvas eklenecektir.",
        centralWidget
    );
    infoLabel->setWordWrap(true);

    layout->addWidget(infoLabel);
    setCentralWidget(centralWidget);

    resize(960, 640);
    setWindowTitle("Sprite AI - Vulkan Desktop Editor");

    setStatusBar(new QStatusBar(this));
    m_statusBarController->attach(statusBar());
    m_statusBarController->showStartupMessage();
}

void MainWindow::initializeVulkan()
{
    try {
        m_vulkanEngine->initialize();
        m_statusBarController->showInfo("Vulkan başarıyla başlatıldı.");
    } catch (const std::exception& ex) {
        m_statusBarController->showError(QString("Vulkan başlatma hatası: %1").arg(ex.what()));
    }
}
