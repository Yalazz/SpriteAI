#include "spriteai/editor/app/QtAIClient.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace spriteai::editor::app {

QtAIClient::QtAIClient(QObject* parent) : QObject(parent) {
    m_endpoint = QUrl("http://localhost:8000/ai/stroke2stroke");
}

    QByteArray QtAIClient::strokeToJson(const spriteai::core::document::Stroke& s) {
    QJsonObject obj;
    obj["rgba"] = QString("#%1").arg(s.rgba, 8, 16, QLatin1Char('0'));
    obj["width"] = s.width;

    QJsonArray pts;
    for (const auto& p : s.points) {
        QJsonObject po;
        po["x"] = p.x;
        po["y"] = p.y;
        po["pressure"] = p.pressure;
        pts.append(po);
    }

    obj["points"] = pts;

    QJsonObject root;
    root["stroke"] = obj;
    root["mode"] = "clean";

    return QJsonDocument(root).toJson(QJsonDocument::Compact);
}


std::vector<spriteai::core::document::Stroke> QtAIClient::strokesFromJson(const QByteArray& bytes) {
    std::vector<spriteai::core::document::Stroke> out;
    const auto doc = QJsonDocument::fromJson(bytes);
    if (!doc.isObject()) return out;

    const auto root = doc.object();
    const auto arr = root.value("strokes").toArray();
    for (const auto& v : arr) {
        if (!v.isObject()) continue;
        const auto o = v.toObject();
        spriteai::core::document::Stroke s;
        bool ok=false;
        auto rgbaStr = o.value("rgba").toString("#ffffffff").remove("#");
        s.rgba = rgbaStr.toUInt(&ok, 16);
        s.width = float(o.value("width").toDouble(6.0));
        const auto pts = o.value("points").toArray();
        for (const auto& pv : pts) {
            if (!pv.isObject()) continue;
            const auto po = pv.toObject();
            s.points.push_back({ float(po.value("x").toDouble()),
                                 float(po.value("y").toDouble()),
                                 float(po.value("pressure").toDouble(1.0)) });
        }
        out.push_back(std::move(s));
    }
    return out;
}

std::vector<spriteai::core::document::Stroke>
QtAIClient::strokeToStrokes(const spriteai::core::document::Stroke& inStroke) {
    // Blocking request for skeleton simplicity.
    QNetworkRequest req(m_endpoint);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto payload = strokeToJson(inStroke);
    QEventLoop loop;
    QNetworkReply* reply = m_net.post(req, payload);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    std::vector<spriteai::core::document::Stroke> out;
    if (reply->error() == QNetworkReply::NoError) {
        out = strokesFromJson(reply->readAll());
    }
    reply->deleteLater();

    // Fallback: echo stroke if AI failed.
    if (out.empty()) out.push_back(inStroke);
    return out;
}

} // namespace spriteai::editor::app
