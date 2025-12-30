#pragma once
#include <QString>

class QApplication;

namespace spriteai::editor::i18n {

    // Dil yükler.
    // true  -> başarıyla yüklendi
    // false -> bulunamadı / yüklenemedi
    bool loadLanguage(QApplication& app, const QString& localeName);
    // örnek: "tr_TR", "en_US"

} // namespace spriteai::editor::i18n
