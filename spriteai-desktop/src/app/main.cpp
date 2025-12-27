#include <QApplication>
#include <QLocale>

#include "app/MainWindow.h"

// Theme
#include "theme/ThemeManager.h"
#include "theme/themes/DarkTheme.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // --------------------------------------------------
    // 🌍 Sistem dili tespiti
    // --------------------------------------------------
    QString systemLang = QLocale::system().name().left(2);
    // örn: "tr_TR" → "tr"

    if (systemLang != "tr")
        systemLang = "en";

    // --------------------------------------------------
    // 🎨 Varsayılan tema (GLOBAL)
    // --------------------------------------------------
    ThemeManager::instance().setTheme(
        std::make_unique<DarkTheme>()
    );

    // --------------------------------------------------
    // 🪟 Ana pencere
    // --------------------------------------------------
    MainWindow w;
    w.setLanguage(systemLang);
    w.show();

    return app.exec();
}
