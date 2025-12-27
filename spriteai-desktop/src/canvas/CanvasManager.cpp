#include "canvas/CanvasManager.h"
#include "core/Document.h"

#include <QDebug>

CanvasManager::CanvasManager() = default;

int CanvasManager::addDocument(const QString& name,
                               std::unique_ptr<Document> document)
{
    documents.push_back({ name, std::move(document) });

    int idx = static_cast<int>(documents.size()) - 1;
    if (documents.size() == 1)
        active = 0;

    qDebug() << "[CanvasManager] addDocument:" << name << "index:" << idx;
    return idx;
}

void CanvasManager::removeDocument(int index)
{
    if (index < 0 || index >= documentCount())
        return;

    documents.erase(documents.begin() + index);

    if (documents.empty())
    {
        active = 0;
        return;
    }

    if (active >= documentCount())
        active = documentCount() - 1;
}

int CanvasManager::documentCount() const
{
    return static_cast<int>(documents.size());
}

Document* CanvasManager::documentAt(int index)
{
    if (index < 0 || index >= documentCount())
        return nullptr;

    return documents[index].document.get();
}

Document& CanvasManager::activeDocument()
{
    return *documents[active].document;
}

void CanvasManager::setActive(int index)
{
    if (index < 0 || index >= documentCount())
        return;

    active = index;
    qDebug() << "[CanvasManager] Active document set to" << active;
}

int CanvasManager::activeIndex() const
{
    return active;
}
