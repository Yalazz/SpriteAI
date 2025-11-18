#include "VulkanWindow.h"
#include "engine/EngineContext.h"

#include <QPainter>
#include <QPaintEvent>

VulkanWindow::VulkanWindow(QWidget* parent, EngineContext* engine)
    : QWidget(parent)
    , m_engine(engine)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
}

VulkanWindow::~VulkanWindow() = default;

void VulkanWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    // For now we simply draw a neutral background.
    // Later this will call into EngineContext to render with Vulkan.
    QPainter painter(this);
    painter.fillRect(rect(), QColor(30, 30, 30));
}

void VulkanWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // In the future we will inform the engine about swapchain resize here.
}
