#include <QApplication>
#include "spriteai/editor/app/MainWindow.h"
#include "spriteai/editor/i18n/I18n.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // default language (can be driven by settings later)
    spriteai::editor::i18n::loadLanguage(app, "tr_TR");

    spriteai::editor::app::MainWindow w;
    w.show();
    return app.exec();
}
