#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <memory>
#include "spriteai/engine/ai/IAIClient.h"


namespace spriteai::core::document
{
    struct Stroke;
}

namespace spriteai::editor::app {

// Qt-based AI client: sends stroke to a FastAPI service and returns strokes.
// This skeleton also supports an offline fallback (echo stroke) if request fails.
class QtAIClient final : public QObject, public spriteai::engine::ai::IAIClient {
    Q_OBJECT
public:
    explicit QtAIClient(QObject* parent=nullptr);

    void setEndpoint(const QUrl& endpoint) { m_endpoint = endpoint; }

    std::vector<spriteai::core::document::Stroke>
    strokeToStrokes(const spriteai::core::document::Stroke& inStroke) override;

private:
    QNetworkAccessManager m_net;
    QUrl m_endpoint;

    static QByteArray strokeToJson(const spriteai::core::document::Stroke& s);
    static std::vector<spriteai::core::document::Stroke> strokesFromJson(const QByteArray& bytes);
};

} // namespace spriteai::editor::app
