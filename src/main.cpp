#include <QApplication>
#include <QLibraryInfo>
#include <QMainWindow>
#include <QTranslator>
#include "menu_page.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator qtAppTranslator;
    if (qtAppTranslator.load(":/res/i18n/de_DE")) {
        qDebug() << "qtAppTranslator ok";
        app.installTranslator(&qtAppTranslator);
    }

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                              QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        qDebug() << "qtBaseTranslator ok";
        app.installTranslator(&qtBaseTranslator);
    }

    MenuPage menu_page;
    menu_page.show();

    return app.exec();
}
