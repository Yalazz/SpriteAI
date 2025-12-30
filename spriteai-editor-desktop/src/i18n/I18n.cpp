#include "spriteai/editor/i18n/I18n.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>

namespace spriteai::editor::i18n {

    bool loadLanguage(QApplication& app, const QString& localeName)
    {
        static QTranslator* translator = nullptr;

        // Önceki dili kaldır
        if (translator) {
            app.removeTranslator(translator);
            delete translator;
            translator = nullptr;
        }

        translator = new QTranslator();

        // tr_* → tr, diğer her şey → en
        const QString lang =
            localeName.startsWith("tr", Qt::CaseInsensitive) ? "tr" : "en";

        const QString baseName =
            QString(":/i18n/spriteai_%1").arg(lang);

        if (translator->load(baseName)) {
            app.installTranslator(translator);
            return true;
        }

        delete translator;
        translator = nullptr;
        return false;
    }

} // namespace spriteai::editor::i18n
