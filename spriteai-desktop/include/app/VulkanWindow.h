#pragma once

#include <QWidget>
#include <memory>

class EngineContext;

class VulkanWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VulkanWindow(QWidget* parent, EngineContext* engine);
    ~VulkanWindow() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    EngineContext* m_engine{nullptr};
};
